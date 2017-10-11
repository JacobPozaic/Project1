#include "Room.h"

int Room::ROOM_SIZE;
int Room::TILE_SIZE;

Room::Room(int x, int y, int dir, Room* last) {
	this->x = x;
	this->y = y;
	this->dir = dir;
	if (dir != -1) this->doors.push_back(dir);			// if the room was not the first room then create a door to the last room
	if (last != NULL) last->addDoor(dir);				// add a door from the last room to this one

	EDGE_OFFSET = (ROOM_SIZE / 2) - TILE_SIZE;

	this->worldX = x * ROOM_SIZE;
	this->worldY = y * ROOM_SIZE;
}

void Room::addDoor(int dir) {
	switch (dir) {
	case 0: this->doors.push_back(2); break;
	case 1: this->doors.push_back(3); break;
	case 2:	this->doors.push_back(0); break;
	case 3: this->doors.push_back(1); break;
	}
}

bool Room::posEquals(int x, int y) {
	if (this->x == x && this->y == y)
		return true;
	return false;
}

FTransform Room::getWorldPosition() {
	return FTransform(FVector(this->x * ROOM_SIZE, this->y * ROOM_SIZE, 0));
}

std::vector<FTransform> Room::getDoorPositions() {
	std::vector<FTransform> doorPositions;
	for (int i = 0; i < this->doors.size(); i++) {
		int x = worldX;
		int y = worldY;
		switch (this->doors[i]) {
		case 0: x -= EDGE_OFFSET; break;
		case 1:	y -= EDGE_OFFSET; break;
		case 2:	x += EDGE_OFFSET; break;
		case 3:	y += EDGE_OFFSET; break;
		}
		FTransform trans_door = FTransform(FVector(x, y, 0));
		trans_door.SetRotation(FQuat::MakeFromEuler({ 0.f, 0.f, 90.0f * (this->doors[i]) }));
		doorPositions.push_back(trans_door);
	}
	return doorPositions;
}

std::vector<FTransform> Room::getWallPositions() {
	std::vector<FTransform> wallPositions;
	int startX = worldX - EDGE_OFFSET - (TILE_SIZE / 2);
	int endX = worldX + EDGE_OFFSET - (TILE_SIZE / 2);
	int startY = worldY - EDGE_OFFSET - (TILE_SIZE / 2);
	int endY = worldY + EDGE_OFFSET - (TILE_SIZE / 2);
	for (int i = 0; i < ROOM_SIZE - TILE_SIZE; i+=TILE_SIZE) {
		FTransform wall = FTransform(FVector(startX + i, startY, 0));
		wallPositions.push_back(wall);
		wall = FTransform(FVector(startX + i, endY, 0));
		wallPositions.push_back(wall);
	}
	for (int i = TILE_SIZE; i < ROOM_SIZE - TILE_SIZE; i += TILE_SIZE) {
		FTransform wall = FTransform(FVector(startX, startY + i, 0));
		wallPositions.push_back(wall);
		wall = FTransform(FVector(endX, startY + i, 0));
		wallPositions.push_back(wall);
	}
	return wallPositions;
}