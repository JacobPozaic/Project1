#include "RoomBuilder.h"

float FRoomBuilder::ROOM_SIZE;
float FRoomBuilder::TILE_SIZE;
float FRoomBuilder::DOOR_WIDTH;
float FRoomBuilder::EDGE_OFFSET;
int32 FRoomBuilder::ROOM_LENGTH;
int32 FRoomBuilder::DOOR_OFF_RNG;

void FRoomBuilder::Init(float room_size, float tile_size, float door_width) {
	ROOM_SIZE = room_size;
	TILE_SIZE = tile_size;
	DOOR_WIDTH = door_width * tile_size;
	EDGE_OFFSET = (room_size / 2) - (tile_size / 2);
	ROOM_LENGTH = room_size / tile_size;
	DOOR_OFF_RNG = ROOM_LENGTH / 2 - door_width;
}

URoom* FRoomBuilder::BuildRoom(int32 x, int32 y, int32 dir, URoom* last) {
	TArray<FCoord> doorPos;
	if (dir != -1) {
		FCoord door;								// stores the location of the door to the previous room
		FCoord door2;
		door.x = 0;									// coordinates allways start at 0
		door.y = 0;									// coordinates allways start at 0
		door.r = dir * 90.0f;						// the rotation on the door
		door2.x = 0;
		door2.y = 0;
		door2.r = door.r + 180.0f;
		int32 door_offset = FMath::RandRange(-DOOR_OFF_RNG, DOOR_OFF_RNG) * TILE_SIZE;	// pick a random tile to shift the door by (so doors are not always centered on each wall)
		switch (dir) {								// determine what wall the door is on
		case 0:										// if the door is on the left wall
			door.x -= EDGE_OFFSET;					// set its x position to be on the left wall
			door.y += door_offset;					// set its y position to be a random spot along the left wall
			door2.x += EDGE_OFFSET;
			door2.y = door.y;
			break;
		case 1:										// if the door is on the above wall	
			door.y -= EDGE_OFFSET;					// set its y position to be on the above wall
			door.x += door_offset;					// set its x position to be a random spot along the above wall
			door2.y += EDGE_OFFSET;
			door2.x = door.x;
			break;
		case 2:										// if the door is on the right wall
			door.x += EDGE_OFFSET;					// set its x position to be on the right wall
			door.y += door_offset;					// set its y position to be a random spot along the right wall
			door2.x -= EDGE_OFFSET;
			door2.y = door.y;
			break;
		case 3:										// if the door is on the bottom wall
			door.y += EDGE_OFFSET;					// set its y position to be on the bottom wall
			door.x += door_offset;					// set its x position to be a random spot along the bottom wall
			door2.y -= EDGE_OFFSET;
			door2.x = door.x;
			break;
		}
		doorPos.Add(door);							// add the door to the list of doors for this room
		last->addDoorEntrance(door2);				// add a door on the previous room that connects to the new door on this room
	}

	/* Place walls */
	TArray<FCoord> wallPos;
	/* Calculate outer wall positions */
	FCoord wall;
	for (int32 i = 0; i < ROOM_LENGTH; i++) {
		/* place wall peice on the top wall */
		wall.x = i;
		wall.y = 0;
		wallPos.Add(wall);
		/* place wall peice on the bottom wall */
		wall.x = i;
		wall.y = ROOM_LENGTH - 1;
		wallPos.Add(wall);
	}
	for (int32 i = 1; i < ROOM_LENGTH - 1; i++) {
		/* place wall peice on the left wall */
		wall.x = 0;
		wall.y = i;
		wallPos.Add(wall);
		/* place wall peice on the right wall */
		wall.x = ROOM_LENGTH - 1;
		wall.y = i;
		wallPos.Add(wall);
	}

	wallPos = FRoomBuilder::UnitToWorld(wallPos);

	/* Create the Room */
	URoom* room = NewObject<URoom>();
	room->Init(x, y, ROOM_SIZE, DOOR_WIDTH, last);
	room->SetWallPositions(wallPos);
	room->SetDoorPositions(doorPos);

	return room;
}

TArray<FCoord> FRoomBuilder::UnitToWorld(TArray<FCoord> coords) {
	TArray<FCoord> worldCoords;
	for (FCoord coord : coords) {
		FCoord world;
		world.x = coord.x * TILE_SIZE - EDGE_OFFSET;
		world.y = coord.y * TILE_SIZE - EDGE_OFFSET;
		worldCoords.Add(world);
	}
	return worldCoords;
}