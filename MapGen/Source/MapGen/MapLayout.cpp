#include "MapLayout.h"
#include <vector>

/** 
 * Helps me keep track of what numbers are what direction.  used when talking about rooms in relation to eachother
 */
enum Direction { NONE = -1, LEFT = 0, ABOVE = 1, RIGHT = 2, DOWN = 3 };

/**
 * An object that represents an individual room in the level
 */
class Room {
public:
	int x;											// coordinate location of this room in the world
	int y;											// coordinate location of this room in the world
	int dir;										// stores the direction to the previous room

	std::vector<int> doors;							// stores the direction each door should be in to adjacent rooms

	Room(int x = 0, int y = 0, int dir = NONE, Room* last = NULL) {
		this->x = x;
		this->y = y;
		this->dir = dir;
		if (dir != NONE) {
			this->doors.push_back(dir);				// if the room was not the first room then create a door to the last room
			last->addDoor(dir);						// add a door from the last room to this one
		}
	}

	/**
	 * Add a door to the previous room in the chain (flip direction)
	 */
	void addDoor(int dir) {
		switch (dir) {
		case LEFT: this->doors.push_back(RIGHT); break;
		case ABOVE: this->doors.push_back(DOWN); break;
		case RIGHT:	this->doors.push_back(LEFT); break;
		case DOWN: this->doors.push_back(ABOVE); break;
		}
	}

	/**
	 * Checks if this room already exists at the given coordinates
	 */
	bool posEquals(int x, int y) {
		if (this->x == x && this->y == y)
			return true;
		return false;
	}
};

/**
 * A light-Weight representation of a Room, used when creating extension rooms to cut down on overhead for potential rooms that are not used. (probably a resurce leak regardless)
 */
class RoomPos {
public:
	int x;											// coordinate location of this room in the world
	int y;											// coordinate location of this room in the world
	int dir;										// stores the direction to the previous room

	RoomPos(int x, int y, int dir) {
		this->x = x;
		this->y = y;
		this->dir = dir;
	}

	/**
	 * Creates a Room object out of the data in this prototype and returns a pointer to it
	 */
	Room* createRoom(Room* lastRoom) {
		return new Room(x, y, dir, lastRoom);
	}
};

std::vector<Room*> rooms;							// stores a pointer to each room in the map

/**
 * Checks if a there is a room that already exists at a given coordinate
 */
bool validLoc(int x, int y) {
	for (int i = 0; i < rooms.size(); i++)
		if ((*rooms[i]).posEquals(x, y))
			return false;
	return true;
}

/**
 * Generates a new room in the main chain of rooms
 */
Room* genNextRoom() {
	Room *last = rooms[rooms.size()-1];		// get a pointer to the most recently created room in the chain
	int dir = FMath::RandRange(0, 2);		// choose a random direction to place the next room in, (dont go DOWN because its easy to get locked in and no extra rooms can be added)
	int x = last->x;						// get the x position of the last room
	int y = last->y;						// get the y position of the last room
	switch (dir) {							// calculate the coordinate of the new room going in the random direction
	case LEFT: x = last->x + 1; break;		// if the new room is to the left, x = last.x + 1
	case ABOVE: y = last->y + 1; break;		// if the new room is above, y = last.y + 1
	case RIGHT:	x = last->x - 1; break;		// if the new room is to the right, x = last.x - 1
	}
	if (!validLoc(x, y))					// check if the location of the new room is not already occupied by another room
		return genNextRoom();				// if the room wasnt valid then try again (random has a good chance not to try the same direction again) [can RARELY cause a stack overflow]
	return new Room(x, y, dir, last);		// return a pointer to the new room
}

/**
 * Generates any rooms that extend off of the main chain.
 * @param current A pointer to the Room that extensions should be added on to
 * @param extend The number of recursive extensions should be made on this tile (dont make this big, like 2..3 is sufficent)
 */
void genExtRooms(Room* current, int extend) {
	std::vector<RoomPos*> validPos;										// stores a pointer to each RoomPosition that is in a valid location

	int x = current->x + 1;
	int y = current->y;
	if (validLoc(x, y)) validPos.push_back(new RoomPos(x, y, LEFT));	// Create a temporary room to the left if there is no room already there
	x = current->x;
	y = current->y + 1;
	if (validLoc(x, y)) validPos.push_back(new RoomPos(x, y, ABOVE));	// Create a temporary room above if there is no room already there
	x = current->x - 1;
	y = current->y;
	if (validLoc(x, y)) validPos.push_back(new RoomPos(x, y, RIGHT));	// Create a temporary room to the right if there is no room already there
	x = current->x;
	y = current->y - 1;
	if (validLoc(x, y)) validPos.push_back(new RoomPos(x, y, DOWN));	// Create a temporary room below if there is no room already there

	int numRooms = FMath::RandRange(0, 2);								// pick a random number of rooms to add onto the current room (0..2)
	while (numRooms-- > 0 && validPos.size() > 0) {						// while we should still add another room and there is another valid location for an adjacent room
		int index = FMath::RandRange(0, validPos.size()-1);				// randomly pick one of the available RoomPos 
		Room *nextRoom = validPos[index]->createRoom(current);			// create a Room from the RoomPos
		rooms.push_back(nextRoom);										// add the new Room's pointer to the list of rooms
		if (extend > 0) genExtRooms(nextRoom, extend-1);				// recursively call self so that each extension room on the main chain has a chance to be extended farther
		validPos[index] = validPos.back();								// overwrite the used RoomPos in the list of availble rooms to remove it from the list
		validPos.pop_back();											// remove the duplicate created by the overwrite
	}
}

AMapLayout::AMapLayout() {
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));	// create a sphere to use as the root of the map generator
	RootComponent = SphereComponent;																		// set the sphere as the root component

	floor_ISMC = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor Instances"));			// create the instanced mesh component for placing floors down for each room
	floor_ISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);			// make it a child of the root component

	door_ISMC = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Door Instances"));				// create the instanced mesh component for placing doors between each room
	door_ISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);				// make it a child of the root component
}

// Called when the game starts
void AMapLayout::BeginPlay() {
	Super::BeginPlay();
	this->RegisterAllComponents();
	floor_ISMC->SetStaticMesh(FloorMesh);
	floor_ISMC->ClearInstances();

	door_ISMC->SetStaticMesh(DoorMesh);
	door_ISMC->ClearInstances();
	
	rooms.clear();
	rooms.push_back(new Room());

	int roomCount = FMath::RandRange(MAIN_CHAIN_MIN, MAIN_CHAIN_RAND + MAIN_CHAIN_MIN);

	for (int i = 0; i < roomCount; i++)
		rooms.push_back(genNextRoom());

	for (int i = 0; i < roomCount - 1; i++)
		genExtRooms(rooms[i], EXT_LENGTH);

	for (int i = 0; i < rooms.size(); i++) {
		Room *cur = rooms[i];
		FTransform trans = FTransform(FVector(cur->x * ROOM_SIZE, cur->y * ROOM_SIZE, 0));
		floor_ISMC->AddInstance(trans);

		int doorCount = cur->doors.size();
		for (int j = 0; j < doorCount; j++) {
			int x = cur->x * ROOM_SIZE;
			int y = cur->y * ROOM_SIZE;
			switch (cur->doors[j]) {
			case 0: x -= DOOR_OFFSET; break;
			case 1:	y -= DOOR_OFFSET; break;
			case 2:	x += DOOR_OFFSET; break;
			case 3:	y += DOOR_OFFSET; break;
			}
			FTransform trans_door = FTransform(FVector(x, y, 0));
			trans_door.SetRotation(FQuat::MakeFromEuler({0.f, 0.f, 90.0f * (cur->doors[j])}));
			door_ISMC->AddInstance(trans_door);
		}
	}
}