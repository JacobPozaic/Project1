#include "Room.h"

int Room::ROOM_SIZE;
int Room::DOOR_OFFSET;

Room::Room(int x, int y, int dir, Room* last) {
	this->x = x;
	this->y = y;
	this->dir = dir;
	if (dir != -1) this->doors.push_back(dir);			// if the room was not the first room then create a door to the last room
	if (last != NULL) last->addDoor(dir);				// add a door from the last room to this one
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
		int x = this->x * ROOM_SIZE;
		int y = this->y * ROOM_SIZE;
		switch (this->doors[i]) {
		case 0: x -= DOOR_OFFSET; break;
		case 1:	y -= DOOR_OFFSET; break;
		case 2:	x += DOOR_OFFSET; break;
		case 3:	y += DOOR_OFFSET; break;
		}
		FTransform trans_door = FTransform(FVector(x, y, 0));
		trans_door.SetRotation(FQuat::MakeFromEuler({ 0.f, 0.f, 90.0f * (this->doors[i]) }));
		doorPositions.push_back(trans_door);
	}
	return doorPositions;
}