#include "Room.h"

URoom::URoom() {}

void URoom::Init(FMapGenParameters* map_layout, float x, float y, URoom* last, bool is_exit) {
	this->pos = Pos(x, y);
	this->width = map_layout->ROOM_WIDTH;
	this->length = map_layout->ROOM_LENGTH;
	this->tile_size = map_layout->TILE_SIZE;
	this->last = last;
	this->exit = is_exit;

	this->world = Pos(x * width * tile_size, y * length * tile_size);
}

void URoom::AddDoorEntrance(FCoord doorPos) {
	this->doors.Add(doorPos);						// add the door to the list of doors for this room
}

bool URoom::PosEquals(int32 x, int32 y) {
	if (pos.x == x && pos.y == y)
		return true;
	return false;
}

FTransform URoom::GetWorldPosition() {
	return FTransform(FVector(world.x, world.y, 0));
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
	TArray<FTransform> transforms;															// stores the FTransform for all doors in this room
	for (FCoord coord : coords) {															// for every door
		FCoord world_offset = coord.Clone();												// move the coordinate from its grid position in the room to its unit position in the room
		world_offset.x -= width / 2;
		world_offset.y -= length / 2;
		world_offset.x *= tile_size;
		world_offset.y *= tile_size;
		world_offset.x += world.x;
		world_offset.y += world.y;
		world_offset.x += tile_size / 2;
		world_offset.y += tile_size / 2;
		FTransform world_trans = FTransform(FVector(world_offset.x, world_offset.y, 0));	// create the FTransform using the world coordinates of the door
		world_trans.SetRotation(FQuat::MakeFromEuler({ 0.f, 0.f, world_offset.r }));		// rotate the door to match the wall it is against
		transforms.Add(world_trans);														// add the FTransform to the list
	}
	return transforms;																		// return the list of FTransforms for all the doors
}

bool URoom::IsStart() {
	return last == NULL;
}

bool URoom::IsExit() {
	return exit;
}