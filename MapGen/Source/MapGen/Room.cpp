#include "Room.h"

URoom::URoom() {}

void URoom::Init(int32 x, int32 y, int32 room_width, int32 room_length, int32 tile_size, URoom* last, bool is_exit) {
	this->x = x;
	this->y = y;
	this->width = room_width;
	this->length = room_length;
	this->tile_size = tile_size;
	this->last = last;
	this->exit = is_exit;

	this->world_x = x * room_width * tile_size;		// calculate this rooms position in world space
	this->world_y = y * room_length * tile_size;	// calculate this rooms position in world space
}

void URoom::AddDoorEntrance(FCoord doorPos) {
	this->doors.Add(doorPos);						// add the door to the list of doors for this room
}

bool URoom::PosEquals(int32 x, int32 y) {
	if (this->x == x && this->y == y)
		return true;
	return false;
}

FTransform URoom::GetWorldPosition() {
	return FTransform(FVector(world_x, world_y, 0));
}

void URoom::SetDoorPositions(TArray<FCoord> doors) {
	this->doors = doors;
}

TArray<FCoord> URoom::GetDoorPositions() {
	return doors;
}

TArray<FTransform> URoom::GetDoorTransforms() {
	return GridToWorld(doors);
}

void URoom::SetWallPositions(TArray<FCoord> walls) {
	this->walls = walls;
}

TArray<FCoord> URoom::GetWallPositions() {
	return walls;
}

TArray<FTransform> URoom::GetWallTransforms() {
	return GridToWorld(walls);
}

TArray<FTransform> URoom::GridToWorld(TArray<FCoord> coords) {
	TArray<FTransform> transforms;												// stores the FTransform for all doors in this room
	for (FCoord coord : coords) {												// for every door
		FCoord world = coord.Clone();											// move the coordinate from its grid position in the room to its unit position in the room
		world.x -= this->width / 2;
		world.y -= this->length / 2;
		world.x *= tile_size;
		world.y *= tile_size;
		world.x += this->world_x;
		world.y += this->world_y;
		world.x += tile_size / 2;
		world.y += tile_size / 2;
		FTransform world_trans = FTransform(FVector(world.x, world.y, 0));		// create the FTransform using the world coordinates of the door
		world_trans.SetRotation(FQuat::MakeFromEuler({ 0.f, 0.f, world.r }));	// rotate the door to match the wall it is against
		transforms.Add(world_trans);											// add the FTransform to the list
	}
	return transforms;															// return the list of FTransforms for all the doors
}

bool URoom::IsStart() {
	return last == NULL;
}

bool URoom::IsExit() {
	return exit;
}