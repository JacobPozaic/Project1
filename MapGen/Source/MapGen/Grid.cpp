#include "Grid.h"

FGrid::FGrid(int32 width, int32 length) {
	this->width = width;
	this->length = length;

	grid.resize(length, std::vector<ETileType>(width, EMPTY));
}

ETileType FGrid::GetTile(int32 x, int32 y) {
	return grid[y][x];
}

void FGrid::SetTiles(TArray<FCoord> tiles, ETileType type) {
	for (FCoord tile : tiles)
		SetTile(tile.x, tile.y, type);
}

bool FGrid::SetTile(int32 x, int32 y, ETileType type) {
	if (grid[y][x] != EMPTY) return false;
	grid[y][x] = type;
	return true;
}

TArray<FCoord> FGrid::GetAllPosOfType(ETileType type) {
	TArray<FCoord> pos;
	for (size_t x = 0; x < grid[0].size(); x++)
		for (size_t y = 0; y < grid.size(); y++)
			if (grid[y][x] == type)
				pos.Add(FCoord(x, y));
	return pos;
}

float FGrid::Coverage() {
	int32 count = 0;
	for (size_t x = 0; x < grid[0].size(); x++)
		for (size_t y = 0; y < grid.size(); y++)
			if (grid[y][x] != EMPTY)
				count++;
	return count / (float)(grid[0].size() * grid.size());
}