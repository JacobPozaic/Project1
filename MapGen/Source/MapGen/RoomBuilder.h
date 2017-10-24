#pragma once
#include "CoreMinimal.h"
#include "Coord.h"
#include "Room.h"
#include "Grid.h"
#include "TileType.h"
#include "Path.h"

static class MAPGEN_API FRoomBuilder {
private:
	static FCoord FRoomBuilder::GetClosestPoint(FCoord start, TArray<FCoord>* points);

public:
	static URoom* BuildRoom(int32 room_x = 0, int32 room_y = 0, int32 room_width = 20, int32 room_length = 20, int32 tile_size = 50, int32 door_offset = 2, int32 dir = -1, URoom* last = NULL);

	static void PopulateRoom(URoom* room);
};
