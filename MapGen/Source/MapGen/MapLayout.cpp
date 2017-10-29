#include "MapLayout.h"

URoom* AMapLayout::GenNextRoom(bool is_exit) {
	URoom* last = rooms.Last();

	/* Choose a random direction to place the next room in, (dont go DOWN because its easy to get locked in and no extra rooms can be added) */
	int32 dir = FMath::RandRange(0, 2);

	/* The position and direction of the room*/
	FCoord pos = FCoord(last->pos.x, last->pos.y, dir);

	/* Calculate the coordinate of the new room going in the random direction; 0 = left, 1 = above, 2 = right */
	switch (dir) {
	case 0: pos.x += 1; break;
	case 1: pos.y += 1; break;
	case 2:	pos.x -= 1; break;
	}

	/* If there is already a room in the spot then try again */
	if (!ValidLoc(pos.x, pos.y)) return GenNextRoom(is_exit);

	/* Create the new room and return it */
	URoom* next = FRoomBuilder::BuildRoom(param, pos, last, is_exit);
	return next;
} 

void AMapLayout::GenExtRooms(URoom* current, int32 extend) {
	/* Stores each Room position that is in a valid location */
	TArray<FCoord> valid_pos;

	/* Pick a random number of rooms to add onto the current room (0..2) */
	int32 num_rooms = FMath::RandRange(0, 2);

	/* Create a temporary room to the left if there is no room already there */
	if (ValidLoc(current->pos.x + 1, current->pos.y)) 
		valid_pos.Add(FCoord(current->pos.x + 1, current->pos.y, 0));

	/* Create a temporary room above if there is no room already there */
	if (ValidLoc(current->pos.x, current->pos.y + 1)) 
		valid_pos.Add(FCoord(current->pos.x, current->pos.y + 1, 1));

	/* Create a temporary room to the right if there is no room already there */
	if (ValidLoc(current->pos.x - 1, current->pos.y))
		valid_pos.Add(FCoord(current->pos.x - 1, current->pos.y, 2));

	/* Create a temporary room below if there is no room already there */
	if (ValidLoc(current->pos.x, current->pos.y - 1))
		valid_pos.Add(FCoord(current->pos.x, current->pos.y - 1, 3));
	
	while (num_rooms-- > 0 && valid_pos.Num() > 0) {							// while we should still add another room and there is another valid location for an adjacent room
		int32 index = FMath::RandRange(0, valid_pos.Num() - 1);					// randomly pick one of the available Rooms
		URoom* next = FRoomBuilder::BuildRoom(param, valid_pos[index], current);	// get the chosen room from the list of valid rooms
		rooms.Add(next);														// add the new Room's pointer to the list of rooms
		if (extend > 0) GenExtRooms(next, extend - 1);							// recursively call self so that each extension room on the main chain has a chance to be extended farther
		valid_pos.RemoveAt(index, 1, true);										// remove it from the list
	}
}

AMapLayout::AMapLayout() {
	USphereComponent* root = CreateDefaultSubobject<USphereComponent>(TEXT("Root Component"));		// create a sphere to use as the root of the map generator
	RootComponent = root;																			// set the sphere as the root component

	floor_ismc = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor Instances"));	// create the instanced mesh component for placing floors down for each room
	floor_ismc->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);	// make it a child of the root component

	door_ismc = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Door Instances"));		// create the instanced mesh component for placing doors between each room
	door_ismc->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);		// make it a child of the root component

	wall_ismc = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall Instances"));		// create the instanced mesh component for placing walls
	wall_ismc->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);		// make it a child of the root component

	param = new FMapGenParameters();
	param->MAIN_CHAIN_MIN = MAIN_CHAIN_MIN;
	param->MAIN_CHAIN_RAND = MAIN_CHAIN_RAND;
	param->EXT_LENGTH = EXT_LENGTH;
	param->ROOM_WIDTH = ROOM_WIDTH;
	param->ROOM_LENGTH = ROOM_LENGTH;
	param->TILE_SIZE = TILE_SIZE;
	param->DOOR_OFFSET = DOOR_OFFSET;
	param->TEMPLATE_SIZE_RATIO = TEMPLATE_SIZE_RATIO;
	param->PATH_POINT_DIST = PATH_POINT_DIST;
	param->ROOM_COVERAGE_MAX = ROOM_COVERAGE_MAX;
	param->RAND_WALL_CHANCE = RAND_WALL_CHANCE;
	param->RAND_WALL_WIDTH_SIZE_RATIO = RAND_WALL_WIDTH_SIZE_RATIO;
	param->RAND_WALL_LENGTH_SIZE_RATIO = RAND_WALL_LENGTH_SIZE_RATIO;
}

void AMapLayout::Init(UStaticMesh* floor_mesh, UStaticMesh* door_mesh, UStaticMesh* wall_mesh) {
	this->floor_mesh = floor_mesh;
	this->door_mesh = door_mesh;
	this->wall_mesh = wall_mesh;
}

void AMapLayout::GenerateMap() {
	this->RegisterAllComponents();

	/* Set the mesh for each ISMC */
	floor_ismc->SetStaticMesh(floor_mesh);
	door_ismc->SetStaticMesh(door_mesh);
	wall_ismc->SetStaticMesh(wall_mesh);
	
	/* Clear all instanced meshes */
	floor_ismc->ClearInstances();
	door_ismc->ClearInstances();
	wall_ismc->ClearInstances();

	/* Clear any rooms stored from the last run */
	rooms.Empty();

	/* Create the start room in the and add it to the list of rooms */
	URoom* start = FRoomBuilder::BuildRoom(param);
	rooms.Add(start);

	/* Calculate how many rooms should be in the main chain */
	int32 room_count = FMath::RandRange(MAIN_CHAIN_MIN, MAIN_CHAIN_RAND + MAIN_CHAIN_MIN);

	/* Generate the main chain of rooms */
	for (int32 i = 1; i <= room_count; i++)
		rooms.Add(GenNextRoom((i == room_count) ? true : false));

	/* Generate extension rooms branching off the main chain of rooms */
	for (int32 i = 0; i < room_count - 1; i++)
		GenExtRooms(rooms[i], EXT_LENGTH);

	/* Populate rooms with obstacles */
	for (int32 i = 0; i < rooms.Num(); i++)
		FRoomBuilder::PopulateRoom(param, rooms[i]);

	/* Start placing the rooms in the world */
	for (int32 i = 0; i < rooms.Num(); i++) {
		URoom* cur = rooms[i];

		/* Place the floor for the room in the world */
		floor_ismc->AddInstance(cur->GetWorldPosition());

		/* Place doors in the world */
		TArray<FTransform> doorPos = cur->GetDoorTransforms();
		for (int32 j = 0; j < doorPos.Num(); j++)
			door_ismc->AddInstance(doorPos[j]);

		/* Place walls around each room */
		TArray<FTransform> wallPos = cur->GetWallTransforms();
		for (int32 j = 0; j < wallPos.Num(); j++)
			wall_ismc->AddInstance(wallPos[j]);
	}
}

bool AMapLayout::ValidLoc(int32 x, int32 y) {
	for (int32 i = 0; i < rooms.Num(); i++)
		if ((*rooms[i]).PosEquals(x, y))
			return false;
	return true;
}