#include "MapLayout.h"

bool AMapLayout::validLoc(int32 x, int32 y) {
	for (int32 i = 0; i < rooms.Num(); i++)
		if ((*rooms[i]).posEquals(x, y))
			return false;
	return true;
}

URoom* AMapLayout::genNextRoom() {
	URoom* last = rooms.Last();					// get a pointer to the most recently created room in the chain
	int32 dir = FMath::RandRange(0, 2);			// choose a random direction to place the next room in, (dont go DOWN because its easy to get locked in and no extra rooms can be added)
	int32 x = last->x;							// get the x position of the last room
	int32 y = last->y;							// get the y position of the last room
	switch (dir) {								// calculate the coordinate of the new room going in the random direction
	case 0: x = last->x + 1; break;				// if the new room is to the left, x = last.x + 1
	case 1: y = last->y + 1; break;				// if the new room is above, y = last.y + 1
	case 2:	x = last->x - 1; break;				// if the new room is to the right, x = last.x - 1
	}
	if (!validLoc(x, y))						// check if the location of the new room is not already occupied by another room
		return genNextRoom();					// if the room wasnt valid then try again (random has a good chance not to try the same direction again) [can RARELY cause a stack overflow]
	URoom* next = NewObject<URoom>(); 
	next->Init(x, y, dir, last);				// get a pointer to the new room
	next->addDoor();							// create a doorway between this room and the last room
	return next;								// return a pointer to the new room
} 

void AMapLayout::genExtRooms(URoom* current, int32 extend) {
	TArray<URoom*> validPos;										// stores a pointer to each RoomPosition that is in a valid location

	int32 x = current->x + 1;
	int32 y = current->y;
	if (validLoc(x, y)) {
		URoom* left = NewObject<URoom>();
		left->Init(x, y, 0, current);
		validPos.Add(left);											// Create a temporary room to the left if there is no room already there
	}
	x = current->x;
	y = current->y + 1;
	if (validLoc(x, y)) {
		URoom* above = NewObject<URoom>();
		above->Init(x, y, 1, current);
		validPos.Add(above);										// Create a temporary room above if there is no room already there
	}
	x = current->x - 1;
	y = current->y;
	if (validLoc(x, y)) {
		URoom* right = NewObject<URoom>();
		right->Init(x, y, 2, current);
		validPos.Add(right);										// Create a temporary room to the right if there is no room already there
	}
	x = current->x;
	y = current->y - 1;
	if (validLoc(x, y)) {
		URoom* below = NewObject<URoom>();
		below->Init(x, y, 3, current);
		validPos.Add(below);										// Create a temporary room below if there is no room already there
	}

	int32 numRooms = FMath::RandRange(0, 2);						// pick a random number of rooms to add onto the current room (0..2)
	while (numRooms-- > 0 && validPos.Num() > 0) {					// while we should still add another room and there is another valid location for an adjacent room
		int32 index = FMath::RandRange(0, validPos.Num() - 1);		// randomly pick one of the available Rooms
		URoom* nextRoom = validPos[index];							// get the chosen room from the list of valid rooms
		nextRoom->addDoor();										// create a door between the new room and the current room
		rooms.Add(nextRoom);										// add the new Room's pointer to the list of rooms
		if (extend > 0) genExtRooms(nextRoom, extend - 1);			// recursively call self so that each extension room on the main chain has a chance to be extended farther
		validPos.RemoveAt(index, 1, true);							// remove it from the list
	}
}

AMapLayout::AMapLayout() {
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));	// create a sphere to use as the root of the map generator
	RootComponent = SphereComponent;																		// set the sphere as the root component

	floor_ISMC = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor Instances"));			// create the instanced mesh component for placing floors down for each room
	floor_ISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);			// make it a child of the root component

	door_ISMC = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Door Instances"));				// create the instanced mesh component for placing doors between each room
	door_ISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);				// make it a child of the root component

	wall_ISMC = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall Instances"));				// create the instanced mesh component for placing walls
	wall_ISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);				// make it a child of the root component
}

// Called when the game starts
void AMapLayout::BeginPlay() {
	/* Setup variables and clearing old information */
	Super::BeginPlay();
	this->RegisterAllComponents();				// Make sure all components are registered

	floor_ISMC->SetStaticMesh(FloorMesh);		// Set the mesh to use for the floor
	floor_ISMC->ClearInstances();				// Clear any instances already on the map
	door_ISMC->SetStaticMesh(DoorMesh);			// Set the mesh to use for the doors
	door_ISMC->ClearInstances();				// Clear any instances already on the map
	wall_ISMC->SetStaticMesh(WallMesh);			// Set the mesh to use for the walls in each room
	wall_ISMC->ClearInstances();				// Clear any instances already on the map

	URoom::ROOM_SIZE = ROOM_SIZE;				// Set the size of rooms for the Room class
	URoom::TILE_SIZE = TILE_SIZE;				// Set the size of tiles for the Room class
	URoom::DOOR_WIDTH = DOOR_WIDTH * TILE_SIZE;	// Set the size of doorways for the Room class
	URoom::EDGE_OFFSET = (ROOM_SIZE / 2) - (TILE_SIZE / 2);

	rooms.Empty();								// Clear any rooms stored from the last run
	URoom* start = NewObject<URoom>();
	start->Init();
	rooms.Add(start);							// Create the first room in the map

	int32 roomCount = FMath::RandRange(MAIN_CHAIN_MIN, MAIN_CHAIN_RAND + MAIN_CHAIN_MIN);		// Calculate how many rooms should be in the main chain of rooms

	// Generate the main chain of rooms
	for (int32 i = 0; i < roomCount; i++)						// For each room that should be created
		rooms.Add(genNextRoom());								// Generate a new room in a valid location adjacent to the previous room

	// Generate extension rooms branching off the main chain of rooms
	for (int32 i = 0; i < roomCount - 1; i++)					// For all rooms in the main chain besides the last room (the exit)
		genExtRooms(rooms[i], EXT_LENGTH);						// Generate rooms that branch off in different directions

	// Start placing the objects in the world
	for (int32 i = 0; i < rooms.Num(); i++) {					// For every room that was generated
		URoom* cur = rooms[i];									// Store a pointer to the current room

		// Place the floor for the room in the world
		floor_ISMC->AddInstance(cur->getWorldPosition());		// Create an instance of the floor mesh in the world location

		// Place doors in the world
		TArray<FTransform> doorPos = cur->getDoorPositions();	// Get the location of all the doors in the room
		for (int32 j = 0; j < doorPos.Num(); j++)				// For each door
			door_ISMC->AddInstance(doorPos[j]);					// Create an instance of it in the world

		// Place walls around each room
		TArray<FTransform> wallPos = cur->getWallPositions();	// Get the location of all the walls in the room
		for (int32 j = 0; j < wallPos.Num(); j++)				// For each wall
			wall_ISMC->AddInstance(wallPos[j]);					// Create an instance of it in the world
	}
}