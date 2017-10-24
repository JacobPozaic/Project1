#pragma once
#include "CoreMinimal.h"
#include "Coord.h"

class MAPGEN_API Pos {
public:
	int32 x;
	int32 y;

	Pos(int32 x = 0, int32 y = 0);
	Pos(FCoord coord);

	bool operator==(const Pos p);

	int32 DistanceTo(Pos target);
};