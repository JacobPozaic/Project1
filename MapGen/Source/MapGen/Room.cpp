#include "Room.h"

int Room::ROOM_SIZE;
int Room::TILE_SIZE;

Room::Room(int x, int y, int dir, Room* last) {
	this->x = x;
	this->y = y;
	this->dir = dir;
	this->last = last;

	EDGE_OFFSET = (ROOM_SIZE / 2) - (TILE_SIZE/2);
	DOOR_OFF_RNG = (ROOM_SIZE / 2) - (4 * TILE_SIZE); // doors are 4 tiles wide + keeps doors atleast 2 tiles from a corner

	this->worldX = x * ROOM_SIZE;
	this->worldY = y * ROOM_SIZE;
}

void Room::addDoor() {
	Coord doorPos;
	doorPos.x = 0;
	doorPos.y = 0;
	int door_offset = FMath::RandRange(-DOOR_OFF_RNG, DOOR_OFF_RNG);
	switch (this->dir) {
	case 0: 
		doorPos.x -= EDGE_OFFSET;
		doorPos.y += door_offset;
		break;
	case 1:	
		doorPos.y -= EDGE_OFFSET;
		doorPos.x += door_offset;
		break;
	case 2:	
		doorPos.x += EDGE_OFFSET;
		doorPos.y += door_offset;
		break;
	case 3:	
		doorPos.y += EDGE_OFFSET;
		doorPos.x += door_offset;
		break;
	}
	
	this->doors.push_back(doorPos);
	last->addDoorEntrance(doorPos);
}

void Room::addDoorEntrance(Coord doorPos) {
	if (abs(doorPos.x) == EDGE_OFFSET)
		doorPos.x = -doorPos.x;
	else if (abs(doorPos.y) == EDGE_OFFSET)
		doorPos.y = -doorPos.y;
	this->doors.push_back(doorPos);
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
		Coord door = doors[i];
		float dir;
		if (door.x == -EDGE_OFFSET) dir = 0.0f;
		else if (door.y == -EDGE_OFFSET) dir = 90.0f;
		else if (door.x == EDGE_OFFSET) dir = 180.0f;
		else if (door.y == EDGE_OFFSET) dir = 270.0f;
		FTransform trans_door = FTransform(FVector(door.x + worldX, door.y + worldY, 0));
		trans_door.SetRotation(FQuat::MakeFromEuler({ 0.f, 0.f, dir }));
		doorPositions.push_back(trans_door);
	}
	return doorPositions;
}

std::vector<FTransform> Room::getWallPositions() {
	std::vector<Coord> wallPositions;
	float startX = worldX - EDGE_OFFSET;
	float endX = worldX + EDGE_OFFSET;
	float startY = worldY - EDGE_OFFSET;
	float endY = worldY + EDGE_OFFSET;
	Coord wall;
	for (int i = 0; i < ROOM_SIZE; i += TILE_SIZE) {
		wall.x = startX + i;
		wall.y = startY;
		wallPositions.push_back(wall);
		wall.x = startX + i;
		wall.y = endY;
		wallPositions.push_back(wall);
	}
	for (int i = TILE_SIZE; i < ROOM_SIZE - TILE_SIZE; i += TILE_SIZE) {
		wall.x = startX;
		wall.y = startY + i;
		wallPositions.push_back(wall);
		wall.x = endX;
		wall.y = startY + i;
		wallPositions.push_back(wall);
	}

	//TODO negotiate where doors are and remove those walls
	std::vector<FTransform> walls;
	for (int i = 0; i < wallPositions.size(); i++) {
		Coord wall = wallPositions[i];
		walls.push_back(FTransform(FVector(wall.x, wall.y, 0)));
	}
	return walls;
}