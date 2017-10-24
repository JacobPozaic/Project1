#include "Pos.h"

Pos::Pos(int32 x, int32 y) {
	this->x = x;
	this->y = y;
}

Pos::Pos(FCoord coord) {
	this->x = coord.x;
	this->y = coord.y;
}

bool Pos::operator==(const Pos p) {
	return this->x == p.x && this->y == p.y;
}

int32 Pos::DistanceTo(Pos target) {
	int x = target.x - this->x;
	int y = target.y - this->y;
	return sqrt(pow(x, 2) + pow(y, 2));
}