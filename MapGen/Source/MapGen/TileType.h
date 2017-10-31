#pragma once
#include "CoreMinimal.h"

/**
* Used to create a grid representing the rooms layout so the pathfinding algorithm can be used to find a safe way through each room
*/
static enum MAPGEN_API ETileType {
	EMPTY, PATH, DOOR, DOORHOLE, WALL, TRAP, ENEMY, KEY, EXIT, TORCH
};
