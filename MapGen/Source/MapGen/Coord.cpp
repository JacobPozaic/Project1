#pragma once
#include "Coord.h"

FCoord::FCoord(float x, float y, float r) {
	this->x = x;
	this->y = y;
	this->r = r;
}

FCoord FCoord::Clone() {
	FCoord coord;
	coord.x = this->x;
	coord.y = this->y;
	coord.r = this->r;
	return coord;
}

float FCoord::DistanceTo(FCoord coord) {
	int x = coord.x - this->x;
	int y = coord.y - this->y;
	return sqrt(pow(x, 2) + pow(y, 2));
}