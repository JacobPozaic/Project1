#pragma once
#include <math.h>

struct FCoord {						// stores the real world coordinates of components in the room, ex: walls, doors
	float x;						// the x position of the object
	float y;						// the y position of the object
	float r;						// the rotation of the object in euler

	FCoord(float x = 0.0f, float y = 0.0f, float r = 0.0f);

	FCoord Clone();

	float DistanceTo(FCoord coord);
};