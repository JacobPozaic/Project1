#pragma once
#include "MapLayout.h"

/**
 * An object that represents an individual room in the level
 */
class MAPGEN_API Room {
private:
	struct Coord {									// stores the real world coordinates of components in the room, ex: walls, doors
		float x;									// the x position of the object
		float y;									// the y position of the object
	};

	int worldX;										// coodinate location of this room in world units
	int worldY;										// coodinate location of this room in world units

	int EDGE_OFFSET;								// constant value for the offset from the center of a room to the edge
	float DOOR_OFF_RNG;								// constant value for how far a door can be offset from the center of a wall

	Room* last;										// stores a pointer to the previous room in the chain

	std::vector<Coord> doors;						// stores the location of each door in the room

	float left = ROOM_SIZE*2;
	float above = ROOM_SIZE*2;
	float right = ROOM_SIZE*2;
	float below = ROOM_SIZE*2;

	/**
	 * Gets the side of a room a door is on
	 */
	int getDoorSide(Coord door);

public:
	int x;											// coordinate location of this room in relation to other rooms
	int y;											// coordinate location of this room in relation to other rooms
	int dir;										// stores the direction to the previous room

	static int ROOM_SIZE;							// constant value for the size in world units of each room
	static int TILE_SIZE;							// constant value for the size of each tile in world units
	static float DOOR_WIDTH;

	Room(int x = 0, int y = 0, int dir = -1, Room* last = NULL);

	/**
	 * Add a door between this room and the previous room in the chain
	 */
	void addDoor();

	/**
	* Add a door between this room and the previous room in the chain
	*/
	void addDoorEntrance(Coord doorPos);

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

	/**
	* Gets the FTansform for every wall belonging to this Room
	*/
	std::vector<FTransform> getWallPositions();
};
