#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Torch.h"
#include "Barrel.h"
#include "Room.h"
#include "RoomBuilder.h"
#include "MapGenParameters.h"
#include "MapLayout.generated.h"

UCLASS()
class MAPGEN_API AMapLayout : public AActor {
	GENERATED_BODY()

public:
	/* The minimum number of rooms from the start to the exit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int32 MAIN_CHAIN_MIN = 5;
	/* The number of extra rooms that could be between the start and exit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int32 MAIN_CHAIN_RAND = 4;
	/* The number of rooms deep that could branch off of each room from the main chain */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int32 EXT_LENGTH = 3;

	/* The number of tiles wide that each room is */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Constraints")
		int32 ROOM_WIDTH = 20;
	/* The number of tiles long that each room is */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Constraints")
		int32 ROOM_LENGTH = 20;
	/* The number of Unreal world units represented by each tile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Constraints")
		int32 TILE_SIZE = 250;
	/* The number of tiles that doors should be offset from corners */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Constraints")
		int32 DOOR_OFFSET = 2;

	/* The ratio between room size and the largest template peice that can be placed in the room */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Generation")
		float TEMPLATE_SIZE_RATIO = 0.5f;
	/* The distance (in tiles) that the safe path should cover before placing random point to detour through */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Generation")
		float PATH_POINT_DIST = 6.0f;
	/* The maximum amount of tiles in a room that should be occupied by obstacles */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Generation")
		float ROOM_COVERAGE_MAX = 0.35f;
	/* The ratio between room width and the largest wall peice that can be placed in the room */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Generation")
		float RAND_WALL_WIDTH_SIZE_RATIO = 0.25f;
	/* The ratio between room length and the largest wall peice that can be placed in the room */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Generation")
		float RAND_WALL_LENGTH_SIZE_RATIO = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMesh* floor_mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMesh* door_mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMesh* wall_mesh;

	UPROPERTY()
		UInstancedStaticMeshComponent* floor_ismc;
	UPROPERTY()
		UInstancedStaticMeshComponent* door_ismc;
	UPROPERTY()
		UInstancedStaticMeshComponent* wall_ismc;

	AMapLayout();
	void Init(UStaticMesh* floor_mesh, UStaticMesh* door_mesh, UStaticMesh* wall_mesh);
	void GenerateMap();

private:
	FMapGenParameters* param;

	/* Stores a pointer to each room in the map */
	TArray<URoom*> rooms;

	/**
	* Generates a new room in the main chain of rooms
	* @param is_exit IF the room is the exit for the map
	*/
	URoom* GenNextRoom(bool is_exit);

	/**
	* Generates any rooms that extend off of the main chain.
	* @param current A pointer to the Room that extensions should be added on to
	* @param extend The number of recursive extensions should be made on this tile (dont make this big, like 2..3 is sufficent)
	*/
	void GenExtRooms(URoom* current, int32 extend);

	/**
	* Checks if a there is a room that already exists at a given coordinate
	* @param x The x position of the room that is being created
	* @param y The y position of the room that is being created
	*/
	bool ValidLoc(int32 x, int32 y);
};