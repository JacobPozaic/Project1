#pragma once
#include "CoreMinimal.h"
#include "Coord.h"

class MAPGEN_API Pos {
public:
	float x;
	float y;

	Pos(float x = 0, float y = 0);
	Pos(FCoord coord);

	bool operator==(const Pos p);

	float DistanceTo(Pos target);
};