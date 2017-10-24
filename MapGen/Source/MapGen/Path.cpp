#include "Path.h"

Path::Path(const FCoord start, const FCoord target, FGrid room) {
	this->start = start;
	this->target = target;
	TArray<Pos> path;
	path.Add(Pos(start));
	path = FindPath(Pos(target), path, room);

	for (Pos p : path) {
		coord_path.Add(FCoord(p.x, p.y));
	}
}

TArray<Path::Pos> Path::FindPath(const Pos target, TArray<Pos> path, FGrid room) {
	Pos cur = path.Last();
	if (cur == target) return path;

	TArray<Probability> valid;

	if (ValidPosition(cur.x, cur.y - 1, path, room)) {
		Pos above = Pos(cur.x, cur.y - 1);
		valid.Add(Probability(above, target));
	}
	if (ValidPosition(cur.x, cur.y + 1, path, room)) {
		Pos below = Pos(cur.x, cur.y + 1);
		valid.Add(Probability(below, target));
	}
	if (ValidPosition(cur.x - 1, cur.y, path, room)) {
		Pos left = Pos(cur.x - 1, cur.y);
		valid.Add(Probability(left, target));
	}
	if (ValidPosition(cur.x + 1, cur.y, path, room)) {
		Pos right = Pos(cur.x + 1, cur.y);
		valid.Add(Probability(right, target));
	}

	TArray<Pos> empty;
	if (valid.Num() == 0) return empty;

	valid.Sort([](const Probability& LHS, const Probability& RHS) { return LHS.prob > RHS.prob; });

	for (Probability p : valid) {
		TArray<Pos> next_tile = path;
		next_tile.Add(p.pos);

		TArray<Pos> next_path = FindPath(target, next_tile, room);

		if (next_path.Num() != 0 && next_path.Last() == target) return next_path;
	}
	return empty;
}

bool Path::ValidPosition(int32 x, int32 y, TArray<Pos> path, FGrid room) {
	if (x < 0) return false;
	if (y < 0) return false;
	if (x >= room.width) return false;
	if (y >= room.length) return false;
	for (Pos pos : path)
		if (pos.x == x && pos.y == y)
			return false;
	return room.GetTile(x, y) == EMPTY || room.GetTile(x, y) == DOORHOLE;
}