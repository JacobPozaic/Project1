#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Room.h"
#include "RoomBuilder.h"
#include "MapLayout.generated.h"

UCLASS(Blueprintable)
class MAPGEN_API AMapLayout : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int32 MAIN_CHAIN_MIN = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int32 MAIN_CHAIN_RAND = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int32 EXT_LENGTH = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int32 ROOM_SIZE = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int32 TILE_SIZE = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int32 DOOR_OFFSET = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		UStaticMesh* FloorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		UStaticMesh* DoorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		UStaticMesh* WallMesh;
	UPROPERTY()
		UInstancedStaticMeshComponent* floor_ISMC;
	UPROPERTY()
		UInstancedStaticMeshComponent* door_ISMC;
	UPROPERTY()
		UInstancedStaticMeshComponent* wall_ISMC;

	AMapLayout();

private:
	TArray<URoom*> rooms;							// stores a pointer to each room in the map

	/**
	 * Checks if a there is a room that already exists at a given coordinate
	 */
	bool validLoc(int32 x, int32 y);

	/**
	* Generates a new room in the main chain of rooms
	*/
	URoom* genNextRoom();

	/**
	* Generates any rooms that extend off of the main chain.
	* @param current A pointer to the Room that extensions should be added on to
	* @param extend The number of recursive extensions should be made on this tile (dont make this big, like 2..3 is sufficent)
	*/
	void genExtRooms(URoom* current, int32 extend);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};