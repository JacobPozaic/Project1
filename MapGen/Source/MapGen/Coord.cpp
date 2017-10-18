#pragma once
#include "Coord.h"

float FCoord::DistanceTo(FCoord coord) {
	int x = coord.x - this->x;
	int y = coord.y - this->y;
	return sqrt(pow(x, 2) + pow(y, 2));
}