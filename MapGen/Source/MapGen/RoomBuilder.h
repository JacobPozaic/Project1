#pragma once
#include "CoreMinimal.h"
#include "Coord.h"
#include "Room.h"
#include "Util.h"
#include "Grid.h"
#include "TileType.h"

static class MAPGEN_API FRoomBuilder {
private:
	static float ROOM_SIZE;				// constant value for the size in world units of each room
	static float TILE_SIZE;				// constant value for the size of each tile in world units
	static float EDGE_OFFSET;			// constant value for the offset from the center of a room to the edge
	static int32 ROOM_LENGTH;			// constant value for number of tiles wide that a room is
	static int32 DOOR_OFF_RNG;			// constant value for how far a door can be offset from the center of a wall

public:
	static void Init(float room_size, float tile_size, float door_width);

	static URoom* BuildRoom(int32 room_x = 0, int32 room_y = 0, int32 dir = -1, URoom* last = NULL);

	static void PopulateRoom(URoom* room);
};
