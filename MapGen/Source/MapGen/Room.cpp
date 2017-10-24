#include "Room.h"

URoom::URoom() {}

void URoom::Init(int32 x, int32 y, float ROOM_SIZE, URoom* last) {
	this->x = x;
	this->y = y;
	this->last = last;

	this->worldX = x * ROOM_SIZE;				// calculate this rooms position in world space
	this->worldY = y * ROOM_SIZE;				// calculate this rooms position in world space
}

void URoom::addDoorEntrance(FCoord doorPos) {
	this->doors.Add(doorPos);					// add the door to the list of doors for this room
}

bool URoom::posEquals(int32 x, int32 y) {
	if (this->x == x && this->y == y)
		return true;
	return false;
}

FTransform URoom::getWorldPosition() {
	return FTransform(FVector(worldX, worldY, 0));
}

void URoom::SetDoorPositions(TArray<FCoord> doors) {
	this->doors = doors;
}

TArray<FCoord> URoom::getDoorPosInRoom() {
	return doors;
}

TArray<FTransform> URoom::getDoorPositions() {
	TArray<FTransform> doorPositions;														// stores the FTransform for all doors in this room
	for (int32 i = 0; i < doors.Num(); i++) {												// for every door
		FCoord door = FUtil::NormalizeLocation(doors[i]);									// move the coordinate from its grid position in the room to its unit position in the room
		FTransform trans_door = FTransform(FVector(door.x + worldX, door.y + worldY, 0));	// create the FTransform using the world coordinates of the door
		trans_door.SetRotation(FQuat::MakeFromEuler({ 0.f, 0.f, door.r }));					// rotate the door to match the wall it is against
		doorPositions.Add(trans_door);														// add the FTransform to the list
	}
	return doorPositions;																	// return the list of FTransforms for all the doors
}

void URoom::SetWallPositions(TArray<FCoord> walls) {
	this->walls = walls;
}

TArray<FTransform> URoom::getWallPositions() {
	/* create an FTransform for each wall and return them in a list */
	TArray<FTransform> wall_pos;
	for (int32 i = 0; i < walls.Num(); i++) {
		FCoord wall = walls[i];
		wall_pos.Add(FTransform(FVector(wall.x + worldX, wall.y + worldY, 0)));
	}
	return wall_pos;
}