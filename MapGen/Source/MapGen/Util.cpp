#include "Util.h"

float FUtil::TILE_SIZE;
float FUtil::EDGE_OFFSET;

void FUtil::Init(float tile_size, float edge_offset) {
	TILE_SIZE = tile_size;
	EDGE_OFFSET = edge_offset;
}

FCoord FUtil::NormalizeLocation(FCoord location) {
	return NormalizeLocation(location.x, location.y, location.r);
}

FCoord FUtil::NormalizeLocation(float x, float y, float r) {
	FCoord pos;
	pos.x = NormalizeValue(x);
	pos.y = NormalizeValue(y);
	pos.r = r;
	return pos;
}

float FUtil::NormalizeValue(float val) {
	return val * TILE_SIZE - EDGE_OFFSET;
}