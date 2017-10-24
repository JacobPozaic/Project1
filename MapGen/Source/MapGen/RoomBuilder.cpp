#include "RoomBuilder.h"

URoom* FRoomBuilder::BuildRoom(int32 room_x, int32 room_y, int32 room_width, int32 room_length, int32 tile_size, int32 door_offset, int32 dir, URoom* last) {
	/* Create the door to the previous room in the chain and add a door to that room that goes to this room */
	TArray<FCoord> door_pos;
	if (dir != -1) {
		FCoord door;						// stores the location of the door to the previous room
		FCoord door2;
		door.r = dir * 90.0f;				// the rotation on the door
		door2.r = door.r + 180.0f;
		switch (dir) {															// determine what wall the door is on
		case 0:																	// if the door is on the left wall
			door.x = 0;															// set its x position to be on the left wall
			door.y = FMath::RandRange(door_offset, room_width - door_offset);	// set its y position to be a random spot along the left wall
			door2.x = room_length - 1;
			door2.y = door.y;
			break;
		case 1:																	// if the door is on the above wall	
			door.y = 0;															// set its y position to be on the above wall
			door.x = FMath::RandRange(door_offset, room_length - door_offset);	// set its x position to be a random spot along the above wall
			door2.y = room_width - 1;
			door2.x = door.x;
			break;
		case 2:																	// if the door is on the right wall
			door.x = room_length - 1;											// set its x position to be on the right wall
			door.y = FMath::RandRange(door_offset, room_width - door_offset);	// set its y position to be a random spot along the right wall
			door2.x = 0;
			door2.y = door.y;
			break;
		case 3:																	// if the door is on the bottom wall
			door.y = room_width - 1;											// set its y position to be on the bottom wall
			door.x = FMath::RandRange(door_offset, room_length - door_offset);	// set its x position to be a random spot along the bottom wall
			door2.y = 0;
			door2.x = door.x;
			break;
		}
		door_pos.Add(door);					// add the door to the list of doors for this room
		last->AddDoorEntrance(door2);		// add a door on the previous room that connects to the new door on this room
	}

	/* Create the Room */
	URoom* room = NewObject<URoom>();
	room->Init(room_x, room_y, room_width, room_length, tile_size, last);
	room->SetDoorPositions(door_pos);

	return room;
}

void FRoomBuilder::PopulateRoom(URoom* room) {
	FGrid* room_layout = new FGrid(room->width, room->length);

	/* Put the door positions on the grid */
	/* NOTE: door does not take into account door width, assumes its allways 3 tiles wide */
	for (FCoord door : room->GetDoorPositions()) {
		room_layout->SetTile(door.x, door.y, DOOR);
		if (((int32)(door.r / 90.0f) % 2) != 0) {	// place doors on the grid adjacent to the center tile of the doorway, rotated depending on what way the door is oriented
			room_layout->SetTile(door.x-1, door.y, DOOR);
			room_layout->SetTile(door.x+1, door.y, DOOR);
		} else {
			room_layout->SetTile(door.x, door.y-1, DOOR);
			room_layout->SetTile(door.x, door.y+1, DOOR);
		}
	}

	/* Place walls */
	/* Calculate outer wall positions */
	for (int32 i = 0; i < room->width; i++) {
		room_layout->SetTile(i, 0, WALL);					// place wall peice on the top wall
		room_layout->SetTile(i, room->length - 1, WALL);	// place wall peice on the bottom wall
	}
	for (int32 i = 1; i < room->length - 1; i++) {
		room_layout->SetTile(0, i, WALL);					// place wall peice on the left wall
		room_layout->SetTile(room->width - 1, i, WALL);		// place wall peice on the right wall
	}

	/* Template wall positions */
	//TODO: design some template rooms

	/* Template trap positions */
	//TODO: traps

	/* Find a safe path between each door */
	// Get door locations

	/* Randomly add other hazards where they will not block the player from navigating the room */

	/* Convert grid to FCoords and put them in the URoom */
	room->SetWallPositions(room_layout->GetAllPosOfType(WALL));
}