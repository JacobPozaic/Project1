#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Components/InstancedStaticMeshComponent.h"
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
		int ROOM_SIZE = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		int TILE_SIZE = 5;
	UPROPERTY(VisibleAnywhere, Category = "Map Generation")
		int DOOR_OFFSET = (ROOM_SIZE / 2) - TILE_SIZE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		UStaticMesh* FloorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
		UStaticMesh* DoorMesh;
	UPROPERTY()
		UInstancedStaticMeshComponent* floor_ISMC;
	UPROPERTY()
		UInstancedStaticMeshComponent* door_ISMC;

	AMapLayout();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};