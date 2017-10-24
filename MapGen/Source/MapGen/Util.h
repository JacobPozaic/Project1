#pragma once
#include "CoreMinimal.h"
#include "Coord.h"

/**
 * 
 */
static class MAPGEN_API FUtil {
private:
	static float TILE_SIZE;
	static float EDGE_OFFSET;

public:
	static void Init(float tile_size, float edge_offset);

	static FCoord NormalizeLocation(FCoord location);
	static FCoord NormalizeLocation(float x, float y, float r);
	static float NormalizeValue(float val);
};
