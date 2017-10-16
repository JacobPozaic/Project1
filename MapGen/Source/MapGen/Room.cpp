#include "Room.h"

int32 URoom::ROOM_SIZE;
int32 URoom::TILE_SIZE;
float URoom::DOOR_WIDTH;
int32 URoom::EDGE_OFFSET;

URoom::URoom() {}

void URoom::Init(int32 x, int32 y, int32 dir, URoom* last) {
	this->x = x;
	this->y = y;
	this->dir = dir;
	this->last = last;

	this->worldX = x * ROOM_SIZE;				// calculate this rooms position in world space
	this->worldY = y * ROOM_SIZE;				// calculate this rooms position in world space

	left = ROOM_SIZE + worldY + DOOR_WIDTH;		// simulate no door (distance is always larger than distance to a door, which would prevent a wall from spawning there)
	above = ROOM_SIZE + worldX + DOOR_WIDTH;	// simulate no door (distance is always larger than distance to a door, which would prevent a wall from spawning there)
	right = ROOM_SIZE + worldY + DOOR_WIDTH;	// simulate no door (distance is always larger than distance to a door, which would prevent a wall from spawning there)
	below = ROOM_SIZE + worldX + DOOR_WIDTH;	// simulate no door (distance is always larger than distance to a door, which would prevent a wall from spawning there)
}

void URoom::addDoor() {
	FCoord doorPos;								// stores the location of a door in world space
	doorPos.x = 0;								// coordinates allways start at 0
	doorPos.y = 0;								// coordinates allways start at 0
	int32 off_rng = ((ROOM_SIZE - DOOR_WIDTH) / TILE_SIZE) / 2;
	int32 door_offset = FMath::RandRange(-off_rng, off_rng) * TILE_SIZE;	// pick a random tile to shift the door by (so doors are not always centered on each wall)
	switch (this->dir) {						// determine what wall the door is on
	case 0:										// if the door is on the left wall
		doorPos.x -= EDGE_OFFSET;				// set its x position to be on the left wall
		doorPos.y += door_offset;				// set its y position to be a random spot along the left wall
		left = doorPos.y + worldY;				// cache the offset location of this door in world cordinates to be used for wall placement
		break;
	case 1:										// if the door is on the above wall	
		doorPos.y -= EDGE_OFFSET;				// set its y position to be on the above wall
		doorPos.x += door_offset;				// set its x position to be a random spot along the above wall
		above = doorPos.x + worldX;				// cache the offset location of this door in world cordinates to be used for wall placement
		break;
	case 2:										// if the door is on the right wall
		doorPos.x += EDGE_OFFSET;				// set its x position to be on the right wall
		doorPos.y += door_offset;				// set its y position to be a random spot along the right wall
		right = doorPos.y + worldY;				// cache the offset location of this door in world cordinates to be used for wall placement
		break;
	case 3:										// if the door is on the bottom wall
		doorPos.y += EDGE_OFFSET;				// set its y position to be on the bottom wall
		doorPos.x += door_offset;				// set its x position to be a random spot along the bottom wall
		below = doorPos.x + worldX;				// cache the offset location of this door in world cordinates to be used for wall placement
		break;
	}

	this->doors.Add(doorPos);					// add the door to the list of doors for this room
	last->addDoorEntrance(doorPos);				// add a door on the previous room that connects to the new door on this room
}

void URoom::addDoorEntrance(FCoord doorPos) {
	/* flip the direction of the door so it lines up with the door on the new room */
	if (FMath::Abs(doorPos.x) == EDGE_OFFSET) doorPos.x = -doorPos.x;
	else if (FMath::Abs(doorPos.y) == EDGE_OFFSET) doorPos.y = -doorPos.y;

	/* cache the offset location of this door in world cordinates to be used for wall placement */
	switch (getDoorSide(doorPos)) {
	case 0: left = doorPos.y + worldY; break;
	case 1:	above = doorPos.x + worldX; break;
	case 2:	right = doorPos.y + worldY; break;
	case 3:	below = doorPos.x + worldX; break;
	}
	this->doors.Add(doorPos);				// add the door to the list of doors for this room
}

int URoom::getDoorSide(FCoord door) {
	if (door.x == -EDGE_OFFSET) return 0;
	else if (door.y == -EDGE_OFFSET) return 1;
	else if (door.x == EDGE_OFFSET) return 2;
	return 3; //else if (door.y == EDGE_OFFSET) , This will never be somthing else because a Door will always have 1 coordinate = EDGE_OFFSET
}

bool URoom::posEquals(int32 x, int32 y) {
	if (this->x == x && this->y == y)
		return true;
	return false;
}

FTransform URoom::getWorldPosition() {
	return FTransform(FVector(this->x * ROOM_SIZE, this->y * ROOM_SIZE, 0));
}

TArray<FTransform> URoom::getDoorPositions() {
	TArray<FTransform> doorPositions;															// stores the FTransform for all doors in this room
	for (int32 i = 0; i < this->doors.Num(); i++) {												// for every door
		FCoord door = doors[i];
		FTransform trans_door = FTransform(FVector(door.x + worldX, door.y + worldY, 0));		// create the FTransform using the world coordinates of the door
		trans_door.SetRotation(FQuat::MakeFromEuler({ 0.f, 0.f, getDoorSide(door) * 90.0f }));	// rotate the door to match the wall it is against
		doorPositions.Add(trans_door);															// add the FTransform to the list
	}
	return doorPositions;																		// return the list of FTransforms for all the doors
}

TArray<FTransform> URoom::getWallPositions() {
	TArray<FCoord> wallPositions;
	/* calculates the range of world coordinates that this room occupies */
	float startX = worldX - EDGE_OFFSET;
	float endX = worldX + EDGE_OFFSET;
	float startY = worldY - EDGE_OFFSET;
	float endY = worldY + EDGE_OFFSET;

	/* Calculate wall positions around the edge of the room */
	/* This is kindof hard to explain step by step but basically it puts a wall on every tile that is on the edge of room
	* if the distance from the cached door location for that wall and that particular peice of wall is less than the width of the door then dont place that wall peice.
	*/
	FCoord wall;
	for (int32 i = 0; i < ROOM_SIZE; i += TILE_SIZE) {
		/* place wall peice on the top wall */
		wall.x = startX + i;
		wall.y = startY;
		if (FMath::Abs(wall.x - above) >= DOOR_WIDTH / 2) wallPositions.Add(wall);
		/* place wall peice on the bottom wall */
		wall.x = startX + i;
		wall.y = endY;
		if (FMath::Abs(wall.x - below) >= DOOR_WIDTH / 2) wallPositions.Add(wall);
	}
	for (int32 i = TILE_SIZE; i < ROOM_SIZE - TILE_SIZE; i += TILE_SIZE) {		// top/bottom walls cover the corner peices so dont also do corners with the left/right
																				/* place wall peice on the left wall */
		wall.x = startX;
		wall.y = startY + i;
		if (FMath::Abs(wall.y - left) >= DOOR_WIDTH / 2) wallPositions.Add(wall);
		/* place wall peice on the right wall */
		wall.x = endX;
		wall.y = startY + i;
		if (FMath::Abs(wall.y - right) >= DOOR_WIDTH / 2) wallPositions.Add(wall);
	}

	/* create an FTransform for each wall and return them in a list */
	TArray<FTransform> walls;
	for (int32 i = 0; i < wallPositions.Num(); i++) {
		FCoord wall = wallPositions[i];
		walls.Add(FTransform(FVector(wall.x, wall.y, 0)));
	}
	return walls;
}