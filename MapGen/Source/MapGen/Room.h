#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Coord.h"
#include "Room.generated.h"

UCLASS()
class MAPGEN_API URoom : public UObject {
	GENERATED_BODY()
private:
	URoom* last;						// stores a pointer to the previous room in the chain

	bool exit;

	TArray<FCoord> walls;				// stores the location of each wall in the room
	TArray<FCoord> doors;				// stores the location of each door in the room

	TArray<FTransform> GridToWorld(TArray<FCoord> coords);

public:
	int32 x;							// coordinate location of this room in relation to other rooms
	int32 y;							// coordinate location of this room in relation to other rooms

	int32 width;						// the number of tiles wide the room is (x axis)
	int32 length;						// the number of tiles long the room is (y axis)

	int32 world_x;						// coodinate location of this room in world units
	int32 world_y;						// coodinate location of this room in world units

	int32 tile_size;

	URoom();

	void Init(int32 x = 0, int32 y = 0, int32 room_width = 20, int32 room_length = 20, int32 tile_size = 50, URoom* last = NULL, bool is_exit = false);

	/**
	* Add a door between this room and the previous room in the chain (only called from other Room objects)
	* @param doorPos The coordinates of the door on the new room, to be reveresed on this room so the doors line up
	*/
	void AddDoorEntrance(FCoord doorPos);

	/* Checks if this room already exists at the given coordinates */
	bool PosEquals(int32 x, int32 y);

	/* Gets the FTransform of where this room should be located in the world space */
	FTransform GetWorldPosition();

	/* Sets the Door positions */
	void SetDoorPositions(TArray<FCoord> doors);

	/* Gets the FCoord for every door belonging to this Room */
	TArray<FCoord> GetDoorPositions();

	/* Gets the FTransform for every door belonging to this Room */
	TArray<FTransform> GetDoorTransforms();

	/* Sets the Wall positions */
	void SetWallPositions(TArray<FCoord> walls);

	/* Gets the FCoord for every wall belonging to this Room */
	TArray<FCoord> GetWallPositions();

	/* Gets the FTransform for every wall belonging to this Room */
	TArray<FTransform> GetWallTransforms();

	/* Gets if this is the start room */
	bool IsStart();

	/* Gets if this is the start room */
	bool IsExit();
};