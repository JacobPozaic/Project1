#pragma once
#include "MapLayout.h"

/**
 * An object that represents an individual room in the level
 */
class MAPGEN_API Room {
public:
	int x;											// coordinate location of this room in the world
	int y;											// coordinate location of this room in the world
	int dir;										// stores the direction to the previous room

	static int ROOM_SIZE;							// constant value for the size in world units of each room
	static int DOOR_OFFSET;							// constant value for the offset from the center of a room to the edge

	std::vector<int> doors;							// stores the direction each door should be in to adjacent rooms

	Room(int x = 0, int y = 0, int dir = -1, Room* last = NULL);

	/**
	 * Add a door to the previous room in the chain (flip direction)
	 */
	void addDoor(int dir);

	/**
	 * Checks if this room already exists at the given coordinates
	 */
	bool posEquals(int x, int y);

	/**
	 * Gets the FTransform of where this room should be located in the world space
	 */
	FTransform getWorldPosition();

	/**
	 * Gets the FTansform for every door belonging to this Room
	 */
	std::vector<FTransform> getDoorPositions();
};
