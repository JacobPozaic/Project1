#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Barrel.generated.h"

UCLASS()
class MAPGEN_API ABarrel : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY()
		USphereComponent* root;
	UPROPERTY()
		UStaticMesh* barrel_mesh_model;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* barrel_mesh;
	ABarrel();

protected:
	virtual void BeginPlay() override;
};
