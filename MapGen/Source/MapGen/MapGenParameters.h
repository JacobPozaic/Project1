#pragma once
#include "CoreMinimal.h"

class MAPGEN_API FMapGenParameters {
public:
	/* The minimum number of rooms from the start to the exit */
	int32 MAIN_CHAIN_MIN;
	/* The number of extra rooms that could be between the start and exit */
	int32 MAIN_CHAIN_RAND;
	/* The number of rooms deep that could branch off of each room from the main chain */
	int32 EXT_LENGTH;

	/* The number of tiles wide that each room is */
	int32 ROOM_WIDTH;
	/* The number of tiles long that each room is */
	int32 ROOM_LENGTH;
	/* The number of Unreal world units represented by each tile */
	int32 TILE_SIZE;
	/* The number of tiles that doors should be offset from corners */
	int32 DOOR_OFFSET;

	/* The ratio between room size and the largest template peice that can be placed in the room */
	float TEMPLATE_SIZE_RATIO;
	/* The distance (in tiles) that the safe path should cover before placing random point to detour through */
	float PATH_POINT_DIST;
	/* The maximum amount of tiles in a room that should be occupied by obstacles */
	float ROOM_COVERAGE_MAX;
	/* The chance that a extra walls are placed in the room */
	float RAND_WALL_CHANCE;
	/* The ratio between room width and the largest wall peice that can be placed in the room */
	float RAND_WALL_WIDTH_SIZE_RATIO;
	/* The ratio between room length and the largest wall peice that can be placed in the room */
	float RAND_WALL_LENGTH_SIZE_RATIO;

	FMapGenParameters();
};
