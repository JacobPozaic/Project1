#pragma once
#include "CoreMinimal.h"
#include "Pos.h"
#include "Coord.h"
#include "Grid.h"

static class MAPGEN_API Path {
private:
	struct Heuristic {
		Pos pos;
		float prob = 100.0f;

		Heuristic::Heuristic(Pos pos, Pos target) {
			this->pos = pos;

			prob -= pos.DistanceTo(target) * 5; // going closer to the target is important
		}
	};

	static TArray<Pos> FindPath(const Pos target, const TArray<Pos>path, const FGrid room);

	static bool ValidPosition(const int32 x, const int32 y, const TArray<Pos> path, const FGrid room);

public:
	static void FindPath(const FCoord start, const FCoord target, FGrid* room);
};
