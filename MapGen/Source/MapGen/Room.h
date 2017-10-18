#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Coord.h"
#include "Room.generated.h"

UCLASS()
class MAPGEN_API URoom : public UObject {
	GENERATED_BODY()
private:
	int32 worldX;						// coodinate location of this room in world units
	int32 worldY;						// coodinate location of this room in world units

	URoom* last;						// stores a pointer to the previous room in the chain

	TArray<FCoord> walls;				// stores the location of each wall in the room
	TArray<FCoord> doors;				// stores the location of each door in the room

	float door_width;

public:
	int32 x;							// coordinate location of this room in relation to other rooms
	int32 y;							// coordinate location of this room in relation to other rooms

	URoom();

	void Init(int32 x = 0, int32 y = 0, float ROOM_SIZE = 200, float DOOR_SIZE = 1.5f, URoom* last = NULL);

	/**
	* Add a door between this room and the previous room in the chain (only called from other Room objects)
	* @param doorPos The coordinates of the door on the new room, to be reveresed on this room so the doors line up
	*/
	void addDoorEntrance(FCoord doorPos);

	/* Checks if this room already exists at the given coordinates */
	bool posEquals(int32 x, int32 y);

	/* Gets the FTransform of where this room should be located in the world space */
	FTransform getWorldPosition();

	/* Sets the Door positions */
	void SetDoorPositions(TArray<FCoord> doors);

	/* Gets the FTansform for every door belonging to this Room */
	TArray<FTransform> getDoorPositions();

	/* Sets the Wall positions */
	void SetWallPositions(TArray<FCoord> walls);

	/* Gets the FTansform for every wall belonging to this Room */
	TArray<FTransform> getWallPositions();
};