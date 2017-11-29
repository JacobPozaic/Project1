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
		/* Place doors on the grid adjacent to the center tile of the doorway, rotated depending on what way the door is oriented */
		room_layout->SetTile(door.x, door.y, DOORHOLE);
		if (((int32)(door.r / 90.0f) % 2) == 0) {
			room_layout->SetTile(door.x, door.y - 1, DOOR);
			room_layout->SetTile(door.x, door.y + 1, DOOR);
		}
		else {
			room_layout->SetTile(door.x - 1, door.y, DOOR);
			room_layout->SetTile(door.x + 1, door.y, DOOR);
		}
	}

	/* Place walls around the edges */
	/* Calculate outer wall positions */
	for (int32 i = 0; i < width; i++) {
		room_layout->SetTile(i, 0, WALL);			// place wall peice on the top wall
		room_layout->SetTile(i, length - 1, WALL);	// place wall peice on the bottom wall
	}
	for (int32 i = 1; i < length - 1; i++) {
		room_layout->SetTile(0, i, WALL);			// place wall peice on the left wall
		room_layout->SetTile(width - 1, i, WALL);	// place wall peice on the right wall
	}

	if (room->IsStart()) {
		/* Place torches */
		room->SetTorchPositions(AddTorches(map_layout, room_layout));
		//TODO: place start
	}
	else if (room->IsExit()) {
		/* Place torches */
		room->SetTorchPositions(AddTorches(map_layout, room_layout));
		//TODO: place exit
	}
	else {
		/* Add template to room */
		int32 random_chance = FMath::RandRange(0, 300);
		int32 half_width = width / 2;
		int32 half_length = length / 2;
		if (random_chance < 100) {
			/* Cylinder in center of room */
			int32 radius = FMath::RandRange(2, ((width < length) ? half_width : half_length)) * map_layout->TEMPLATE_SIZE_RATIO;
			if (radius % 2 == 1) radius++;
			TArray<FCoord> cylinder;
			for (int32 x = 0; x <= radius; x++) {
				for (int32 y = 0; y <= radius; y++) {
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

		/* Find a safe path between each door */
		for (int32 i = 0; i < doors.Num() - 1; i++) {
			for (int32 j = i + 1; j < doors.Num(); j++) {
				int32 randomize_path = FMath::RandRange(0, (int32)(doors[i].DistanceTo(doors[j]) / map_layout->PATH_POINT_DIST));
				if (randomize_path > 0) {
					int32 min_x;
					int32 min_y;
					int32 max_x;
					int32 max_y;
					if (doors[i].x < doors[j].x) {
						min_x = doors[i].x + 1;
						max_x = doors[j].x - 1;
					}
					else {
						min_x = doors[j].x + 1;
						max_x = doors[i].x - 1;
					}
					if (doors[i].y < doors[j].y) {
						min_y = doors[i].y + 1;
						max_y = doors[j].y - 1;
					}
					else {
						min_y = doors[j].y + 1;
						max_y = doors[i].y - 1;
					}
					TArray<Pos> points;
					for (int32 k = 0; k < randomize_path; k++) {
						Pos pos = Pos(FMath::RandRange(min_x, max_x), FMath::RandRange(min_y, max_y));
						ETileType tile = room_layout->GetTile(pos.x, pos.y);
						if (tile == EMPTY || tile == PATH)
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
		room->SetTorchPositions(AddTorches(map_layout, room_layout));

		/* Randomly add other hazards where they will not block the player from navigating the room */
		while (room_layout->Coverage() < map_layout->ROOM_COVERAGE_MAX) {
			int32 rand = FMath::RandRange(0, 300);
			if (rand < 50) {
				/* Place barrels */
				TArray<Pos> pos;
				for (int32 x = 1; x < map_layout->ROOM_WIDTH - 1; x++)
					for (int32 y = 1; y < map_layout->ROOM_LENGTH - 1; y++)
						if (room_layout->GetTile(x, y) == EMPTY)
							pos.Add(Pos(x, y));
				int32 rand = FMath::RandRange(0, pos.Num() - 1);
				room_layout->SetTile(pos[rand].x, pos[rand].y, BARREL);
			}
			else if (rand < 100) {
				/* Place crates */
				TArray<Pos> pos;
				for (int32 x = 1; x < map_layout->ROOM_WIDTH - 1; x++)
					for (int32 y = 1; y < map_layout->ROOM_LENGTH - 1; y++)
						if (room_layout->GetTile(x, y) == EMPTY)
							pos.Add(Pos(x, y));
				int32 rand = FMath::RandRange(0, pos.Num() - 1);
				room_layout->SetTile(pos[rand].x, pos[rand].y, CRATE);
			}
			else if (rand < 250) {
				/* Place walls */
				int32 wall_width = FMath::RandRange(1, (int32)(width *  map_layout->RAND_WALL_WIDTH_SIZE_RATIO));
				int32 wall_length = FMath::RandRange(1, (int32)(length *  map_layout->RAND_WALL_LENGTH_SIZE_RATIO));
				Pos pos;
				bool invalid = true;
				while (invalid) {
					invalid = false;
					pos = Pos(FMath::RandRange(1, width - wall_width - 1),
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
		}
	}

	/* Convert grid to FCoords and put them in the URoom */
	room->SetWallPositions(room_layout->GetAllPosOfType(WALL));
	room->SetBarrelPositions(room_layout->GetAllPosOfType(BARREL));
	room->SetCratePositions(room_layout->GetAllPosOfType(CRATE));
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

TArray<FCoord> FRoomBuilder::AddTorches(FMapGenParameters* map_layout, FGrid* room_layout) {
	/* Find all valid positions for a torch */
	TArray<FCoord> torch_possible_locations;
	for (int32 x = 1; x < map_layout->ROOM_WIDTH - 1; x++) {
		for (int32 y = 1; y < map_layout->ROOM_LENGTH - 1; y++) {
			if (room_layout->GetTile(x, y) == EMPTY) {
				int32 rotation = 0;
				if (room_layout->GetTile(x + 1, y) == WALL)
					rotation = 1;
				else if (room_layout->GetTile(x - 1, y) == WALL)
					rotation = -1;
				else if (room_layout->GetTile(x, y + 1) == WALL)
					rotation = 2;
				else if (room_layout->GetTile(x, y - 1) == WALL)
					rotation = 0;
				else continue;

				torch_possible_locations.Add(FCoord(x, y, rotation * 90.0f));
			}
		}
	}

	TArray<FCoord> shuffled;
	/* Shuffle the possible torch locations so that torches are more randomly distributed and remove 25% of the possible torches */
	while (torch_possible_locations.Num() > 0) {
		if (FMath::RandRange(0, 100) <= 25) {
			torch_possible_locations.RemoveAt(0);
			continue;
		}
		int32 index = FMath::RandRange(0, torch_possible_locations.Num() - 1);
		shuffled.Add(torch_possible_locations[index]);
		torch_possible_locations.RemoveAt(index);
	}

	/* Check if the torch is in range of any other torches, if not then place it */
	TArray<FCoord> torch_locations;
	for (FCoord torch : shuffled) {
		bool torch_in_range = false;
		TArray<FCoord> torches = room_layout->GetAllPosOfType(TORCH);
		for (FCoord existing_torch : torches) {
			if (torch.DistanceTo(existing_torch) <= 5.0f) {
				torch_in_range = true;
				break;
			}
		}
		/* No idea why this doesnt work
		for (int32 range_x = torch.x - torch_range; range_x < torch.x + torch_range; range_x++) {
			if (torch_in_range) break;
			for (int32 range_y = torch.y - torch_range; range_y < torch.y + torch_range; range_y++) {
				if (torch_in_range) break;
				if (range_x > 0 && range_x < map_layout->ROOM_WIDTH && range_y > 0 && range_y < map_layout->ROOM_LENGTH)
					if (room_layout->GetTile(range_x, range_y) == TORCH)
						torch_in_range = true;
			}
		} */

		if (!torch_in_range) {
			room_layout->SetTile(torch.x, torch.y, TORCH);
			torch_locations.Add(torch);
		}
	}
	return torch_locations;
}