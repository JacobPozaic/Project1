#include "Path.h"

void Path::FindPath(Pos start, Pos target, FGrid* room) {
	TArray<Pos> path;
	path.Add(start);
	for (Pos p : FindPath(target, path, *room))
		room->SetTile(p.x, p.y, PATH);
}

TArray<Pos> Path::FindPath(Pos target, TArray<Pos> path, FGrid room) {
	Pos cur = path.Last();
	if (cur == target) return path;

	TArray<Heuristic> valid_moves;

	if (ValidPosition(cur.x, cur.y - 1, path, room)) {
		Pos above = Pos(cur.x, cur.y - 1);
		valid_moves.Add(Heuristic(above, target));
	}
	if (ValidPosition(cur.x, cur.y + 1, path, room)) {
		Pos below = Pos(cur.x, cur.y + 1);
		valid_moves.Add(Heuristic(below, target));
	}
	if (ValidPosition(cur.x - 1, cur.y, path, room)) {
		Pos left = Pos(cur.x - 1, cur.y);
		valid_moves.Add(Heuristic(left, target));
	}
	if (ValidPosition(cur.x + 1, cur.y, path, room)) {
		Pos right = Pos(cur.x + 1, cur.y);
		valid_moves.Add(Heuristic(right, target));
	}

	TArray<Pos> empty;
	if (valid_moves.Num() == 0) return empty;

	valid_moves.Sort([](const Heuristic& LHS, const Heuristic& RHS) { return LHS.prob > RHS.prob; });

	for (Heuristic p : valid_moves) {
		TArray<Pos> next_tile;
		for (Pos path_part : path) 
			next_tile.Add(path_part);
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
	return room.GetTile(x, y) == EMPTY || room.GetTile(x, y) == DOORHOLE || room.GetTile(x, y) == PATH;
}