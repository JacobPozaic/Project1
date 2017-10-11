#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <vector>
#include "Room.h"
#include "MapLayout.generated.h"

UCLASS()
class MAPGEN_API AMapLayout : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int MAIN_CHAIN_MIN = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int MAIN_CHAIN_RAND = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int EXT_LENGTH = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int ROOM_SIZE = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int TILE_SIZE = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		float DOOR_WIDTH = 1.5f;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};