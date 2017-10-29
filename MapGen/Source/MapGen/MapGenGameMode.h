#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/GameModeBase.h"

#include "PlayerCharacter.h"
#include "MapLayout.h"

#include "MapGenGameMode.generated.h"

UCLASS()
class MAPGEN_API AMapGenGameMode : public AGameModeBase {
	GENERATED_BODY()

private:
	UStaticMesh* floor_mesh;
	UStaticMesh* door_mesh;
	UStaticMesh* wall_mesh;
	AMapLayout* map_layout;

public:
	AMapGenGameMode(const FObjectInitializer& ObjectInitializer);
	
	virtual void StartPlay() override;
};
