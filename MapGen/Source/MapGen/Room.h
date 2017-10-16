#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Room.generated.h"

USTRUCT()
struct FCoord {						// stores the real world coordinates of components in the room, ex: walls, doors
	GENERATED_BODY()
	float x;						// the x position of the object
	float y;						// the y position of the object
};

UCLASS()
class MAPGEN_API URoom : public UObject {
	GENERATED_BODY()
private:
	int32 worldX;						// coodinate location of this room in world units
	int32 worldY;						// coodinate location of this room in world units

	URoom* last;						// stores a pointer to the previous room in the chain

	TArray<FCoord> doors;				// stores the location of each door in the room

	float left;							// stores the position of the door on the left wall if there is one
	float above;						// stores the position of the door on the upper wall if there is one
	float right;						// stores the position of the door on the right wall if there is one
	float below;						// stores the position of the door on the lower wall if there is one

	/**
	* Gets the side of a room a door is on
	*/
	int32 getDoorSide(FCoord door);

public:
	int32 x;							// coordinate location of this room in relation to other rooms
	int32 y;							// coordinate location of this room in relation to other rooms
	int32 dir;							// stores the direction to the previous room

	static int32 ROOM_SIZE;				// constant value for the size in world units of each room
	static int32 TILE_SIZE;				// constant value for the size of each tile in world units
	static float DOOR_WIDTH;			// constant value for width of a door
	static int32 EDGE_OFFSET;			// constant value for the offset from the center of a room to the edge
	static int32 DOOR_OFF_RNG;			// constant value for how far a door can be offset from the center of a wall

	URoom();

	void Init(int32 x = 0, int32 y = 0, int32 dir = -1, URoom* last = NULL);

	/**
	* Add a door between this room and the previous room in the chain
	*/
	void addDoor();

	/**
	* Add a door between this room and the previous room in the chain (only called from other Room objects)
	* @param doorPos The coordinates of the door on the new room, to be reveresed on this room so the doors line up
	*/
	void addDoorEntrance(FCoord doorPos);

	/**
	* Checks if this room already exists at the given coordinates
	*/
	bool posEquals(int32 x, int32 y);

	/**
	* Gets the FTransform of where this room should be located in the world space
	*/
	FTransform getWorldPosition();

	/**
	* Gets the FTansform for every door belonging to this Room
	*/
	TArray<FTransform> getDoorPositions();

	/**
	* Gets the FTansform for every wall belonging to this Room
	*/
	TArray<FTransform> getWallPositions();
};