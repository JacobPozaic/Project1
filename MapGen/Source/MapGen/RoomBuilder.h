#pragma once
#include "CoreMinimal.h"
#include "Coord.h"
#include "Pos.h"
#include "Room.h"
#include "Grid.h"
#include "TileType.h"
#include "Path.h"
#include "MapGenParameters.h"

static class MAPGEN_API FRoomBuilder {
private:
	/**
	 * Find the closest path detour point to the current position.
	 *
	 * @param start The position to find the closest point to
	 * @param points The array of detour points to search through
	 */
	static Pos FRoomBuilder::GetClosestPoint(Pos start, TArray<Pos>* points);

public:
	/**
	 * Creates a URoom and returns a pointer to it.
	 *
	 * @param map_layout The map generator context to use when generating this room
	 * @param pos The location and direction of the URoom
	 * @param last A pointer to the URoom that this URoom extends from
	 * @param is_exit If this URoom is the exit for the level
	 */
	static URoom* BuildRoom(FMapGenParameters* map_layout, FCoord pos = FCoord(0, 0, -1), URoom* last = NULL, bool is_exit = false);

	/**
	 * Generates the layout for a URoom.
	 *
	 * @param map_layout The map generator context to use when generating this room
	 * @param room The room to populate with obstacles
	 */
	static void PopulateRoom(FMapGenParameters* map_layout, URoom* room);
};
