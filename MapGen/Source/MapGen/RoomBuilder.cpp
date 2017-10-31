#include "RoomBuilder.h"

URoom* FRoomBuilder::BuildRoom(FMapGenParameters* map_layout, FCoord pos, URoom* last, bool is_exit) {
	/* Create the door to the previous room in the chain and add a door to that room that goes to this room */
	TArray<FCoord> door_pos;
	if (pos.r != -1) {
		FCoord door_current;
		FCoord door_previous;
		/* Convert the door rotation from cardinal to Euler and flip door2's rotation*/
		door_current.r = pos.r * 90.0f;
		door_previous.r = door_current.r + 180.0f;
		switch ((int32)pos.r) {
		case 0:
			/* Door is on the left wall */
			door_current.x = 0;
			door_current.y = FMath::RandRange(map_layout->DOOR_OFFSET, map_layout->ROOM_WIDTH - map_layout->DOOR_OFFSET);
			door_previous.x = map_layout->ROOM_LENGTH - 1;
			door_previous.y = door_current.y;
			break;
		case 1:
			/* Door is on the top wall */
			door_current.y = 0;
			door_current.x = FMath::RandRange(map_layout->DOOR_OFFSET, map_layout->ROOM_LENGTH - map_layout->DOOR_OFFSET);
			door_previous.y = map_layout->ROOM_WIDTH - 1;
			door_previous.x = door_current.x;
			break;
		case 2:
			/* Door is on the right wall */
			door_current.x = map_layout->ROOM_LENGTH - 1;
			door_current.y = FMath::RandRange(map_layout->DOOR_OFFSET, map_layout->ROOM_WIDTH - map_layout->DOOR_OFFSET);
			door_previous.x = 0;
			door_previous.y = door_current.y;
			break;
		case 3:
			/* Door is on the bottom wall */
			door_current.y = map_layout->ROOM_WIDTH - 1;
			door_current.x = FMath::RandRange(map_layout->DOOR_OFFSET, map_layout->ROOM_LENGTH - map_layout->DOOR_OFFSET);
			door_previous.y = 0;
			door_previous.x = door_current.x;
			break;
		}
		/* Add the doorway between this room and the last */
		door_pos.Add(door_current);
		last->AddDoorEntrance(door_previous);
	}

	/* Create the Room */
	URoom* room = NewObject<URoom>();
	room->Init(map_layout, pos.x, pos.y, last, is_exit);
	room->SetDoorPositions(door_pos);
	return room;
}

void FRoomBuilder::PopulateRoom(FMapGenParameters* map_layout, URoom* room) {
	int32 width = room->width;
	int32 length = room->length;
	TArray<FCoord> doors = room->GetDoorPositions();
	FGrid* room_layout = new FGrid(width, length);

	/* Put the door positions on the grid */
	/* NOTE: door does not take into account door width, assumes its allways 3 tiles wide */
	for (FCoord door : doors) {
		room_layout->SetTile(door.x, door.y, DOORHOLE);
		if (((int32)(door.r / 90.0f) % 2) == 0) {	// place doors on the grid adjacent to the center tile of the doorway, rotated depending on what way the door is oriented
			room_layout->SetTile(door.x, door.y - 1, DOOR);
			room_layout->SetTile(door.x, door.y + 1, DOOR);
		} else {
			room_layout->SetTile(door.x - 1, door.y, DOOR);
			room_layout->SetTile(door.x + 1, door.y, DOOR);
		}
	}

	/* Place walls */
	/* Calculate outer wall positions */
	for (int32 i = 0; i < width; i++) {
		room_layout->SetTile(i, 0, WALL);					// place wall peice on the top wall
		room_layout->SetTile(i, length - 1, WALL);	// place wall peice on the bottom wall
	}
	for (int32 i = 1; i < length - 1; i++) {
		room_layout->SetTile(0, i, WALL);					// place wall peice on the left wall
		room_layout->SetTile(width - 1, i, WALL);		// place wall peice on the right wall
	}

	if (room->IsStart()) {
		//TODO: place start
	} else if(room->IsExit()) {
		//TODO: place exit
	} else {
		/* Template wall positions */
		int32 random_chance = FMath::RandRange(0, 1000);
		int32 half_width = width / 2;
		int32 half_length = length / 2;
		if (random_chance < 100) {
			/* Cylinder in center of room */
			int32 radius = FMath::RandRange(2, ((width < length) ? half_width : half_length)) * map_layout->TEMPLATE_SIZE_RATIO;
			if (radius % 2 == 1) radius++;
			TArray<FCoord> cylinder;
			for (int x = 0; x <= radius; x++) {
				for (int y = 0; y <= radius; y++) {
					if (x * x + y * y <= radius * radius) {
						cylinder.Add(FCoord(half_width + x, half_length + y));
						cylinder.Add(FCoord(half_width - x - 1, half_length + y));
						cylinder.Add(FCoord(half_width + x, half_length - y - 1));
						cylinder.Add(FCoord(half_width - x - 1, half_length - y - 1));
					}
				}
			}
			room_layout->SetTiles(cylinder, WALL);
		}
		else if (random_chance < 200) {
			/* Square in room */
			int32 sq_width = FMath::RandRange(1, (int32)(half_width * map_layout->TEMPLATE_SIZE_RATIO));
			int32 sq_length = FMath::RandRange(1, (int32)(half_length * map_layout->TEMPLATE_SIZE_RATIO));
			TArray<FCoord> square;
			for (int x = 0; x < sq_width; x++)
				for (int y = 0; y < sq_length; y++)
					square.Add(FCoord(x + half_width, y + half_length));
			room_layout->SetTiles(square, WALL);
		}
		else {

		}

		/* Template trap positions */
		//TODO: traps

		/* Find a safe path between each door */
		for (int i = 0; i < doors.Num() - 1; i++) {
			for (int j = i + 1; j < doors.Num(); j++) {
				int randomize_path = FMath::RandRange(0, (int32)(doors[i].DistanceTo(doors[j]) / map_layout->PATH_POINT_DIST));
				if (randomize_path > 0) {
					int min_x;
					int min_y;
					int max_x;
					int max_y;
					if (doors[i].x < doors[j].x) {
						min_x = doors[i].x + 1;
						max_x = doors[j].x - 1;
					} else {
						min_x = doors[j].x + 1;
						max_x = doors[i].x - 1;
					}
					if (doors[i].y < doors[j].y) {
						min_y = doors[i].y + 1;
						max_y = doors[j].y - 1;
					} else {
						min_y = doors[j].y + 1;
						max_y = doors[i].y - 1;
					}
					TArray<Pos> points;
					for (int k = 0; k < randomize_path; k++) {
						Pos pos = Pos(FMath::RandRange(min_x, max_x), FMath::RandRange(min_y, max_y));
						if (room_layout->GetTile(pos.x, pos.y) == EMPTY || room_layout->GetTile(pos.x, pos.y) == PATH)
							points.Add(pos);
					}
					if (points.Num() != 0) {
						Pos next = GetClosestPoint(doors[i], &points);
						Path::FindPath(doors[i], next, room_layout);
						while (points.Num() > 0) {
							Pos last = next;
							next = GetClosestPoint(last, &points);
							Path::FindPath(last, next, room_layout);
						}
						Path::FindPath(next, doors[j], room_layout);
					}
					else Path::FindPath(doors[i], doors[j], room_layout);
				}
				else Path::FindPath(doors[i], doors[j], room_layout);
			}
		}

		/* Place torches */
		int torch_range = 4;
		for (int x = 1; x < map_layout->ROOM_WIDTH - 1; x++) {
			for (int y = 1; y < map_layout->ROOM_LENGTH - 1; y++) {
				if (room_layout->GetTile(x, y) != EMPTY)
					continue;

				int rotation = -1;	//TODO: rotation
				if (room_layout->GetTile(x + 1, y) == WALL)
					rotation = 0;
				else if (room_layout->GetTile(x - 1, y) == WALL)
					rotation = 1;
				else if (room_layout->GetTile(x, y + 1) == WALL)
					rotation = 2;
				else if (room_layout->GetTile(x, y - 1) == WALL)
					rotation = 3;
				else continue;

				bool torch_in_range = false;
				for (int range_x = x - torch_range; range_x < x + torch_range; range_x++) {
					for (int range_y = y - torch_range; range_y < y + torch_range; range_y++) {
						if (torch_in_range) break;
						if (range_x >= 1 && range_x <= map_layout->ROOM_WIDTH - 1 && range_y >= 1 && range_y <= map_layout->ROOM_LENGTH - 1)
							if (room_layout->GetTile(range_x, range_y) == TORCH)
								torch_in_range = true;
					}
				}
					
				if (!torch_in_range && FMath::RandRange(0.0f, 1.0f) < 0.2f) {
					room_layout->SetTile(x, y, TORCH);
				}
			}
		}

		/* Randomly add other hazards where they will not block the player from navigating the room */
		/* It might be better to not have randomly generated walls */
		while (room_layout->Coverage(WALL) < map_layout->ROOM_COVERAGE_MAX) {
			if (FMath::RandRange(0.0f, 1.0f) > map_layout->RAND_WALL_CHANCE) { //todo: pretty sure this is backwards
				int32 wall_width = FMath::RandRange(1, (int32)(width *  map_layout->RAND_WALL_WIDTH_SIZE_RATIO));
				int32 wall_length = FMath::RandRange(1, (int32)(length *  map_layout->RAND_WALL_LENGTH_SIZE_RATIO));
				FCoord pos;
				bool invalid = true;
				while (invalid) {
					invalid = false;
					pos = FCoord(FMath::RandRange(1, width - wall_width - 1),
						FMath::RandRange(1, length - wall_length - 1));

					for (FCoord door : doors) {
						switch ((int32)(door.r / 90.0f) % 4) {
						case 0:	// Left
							if (door.x + 1 == pos.x && door.y >= pos.y && door.y <= pos.y + wall_length)
								invalid = true;
							break;
						case 1:	// Above
							if (door.y + 1 == pos.y && door.x >= pos.x && door.x <= pos.x + wall_width)
								invalid = true;
							break;
						case 2:	// Right
							if (door.x - 1 == pos.x + wall_width && door.y >= pos.y && door.y <= pos.y + wall_length)
								invalid = true;
							break;
						case 3:	// Below
							if (door.y - 1 == pos.y + wall_length && door.x >= pos.x && door.x <= pos.x + wall_width)
								invalid = true;
							break;
						}
					}
				}

				for (int32 i = 0; i < wall_width; i++)
					for (int32 j = 0; j < wall_length; j++)
						room_layout->SetTile(pos.x + i, pos.y + j, WALL);
			}
			else break;
		}
	}
	
	/* Convert grid to FCoords and put them in the URoom */
	room->SetWallPositions(room_layout->GetAllPosOfType(WALL));
	room->SetTorchPositions(room_layout->GetAllPosOfType(TORCH));
}

Pos FRoomBuilder::GetClosestPoint(Pos start, TArray<Pos>* points) {
	Pos closest = (*points)[0];
	int32 dist_close = start.DistanceTo(closest);
	int32 index = 0;
	for (int k = 1; k < (*points).Num(); k++) {
		int dist_point = start.DistanceTo((*points)[k]);
		if (dist_point < dist_close) {
			closest = (*points)[k];
			dist_close = dist_point;
			index = k;
		}
	}
	points->RemoveAt(index);
	return closest;
}