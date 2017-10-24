#pragma once
#include "CoreMinimal.h"
#include "Coord.h"
#include "Grid.h"

/**
 * 
 */
class MAPGEN_API Path {
private:
	struct Pos {
		int32 x;
		int32 y;

		Pos::Pos(int32 x = 0, int32 y = 0) {
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
	};

	struct Probability {
		Pos pos;
		float prob = 100.0f;

		Probability::Probability(Pos pos, Pos target) {
			this->pos = pos;

			prob -= pos.DistanceTo(target) * 5; // going closer to the target is important
		}
	};

	TArray<Pos> FindPath(const Pos target, TArray<Pos>path, FGrid room);

	bool ValidPosition(int32 x, int32 y, TArray<Pos> path, FGrid room);

public:
	FCoord start;
	FCoord target;
	TArray<FCoord> coord_path;

	Path(const FCoord start, const FCoord target, const FGrid room);
};
