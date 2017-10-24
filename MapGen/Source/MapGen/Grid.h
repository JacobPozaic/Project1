#pragma once
#include "CoreMinimal.h"
#include <vector>
#include "TileType.h"
#include "Coord.h"

class MAPGEN_API FGrid {
private:
	std::vector<std::vector<ETileType>> grid;	// array to keep track of positions of each tile in a room

public:
	FGrid(int32 width, int32 length);

	bool SetTile(int32 x, int32 y, ETileType type);

	TArray<FCoord> GetAllPosOfType(ETileType type);
};
