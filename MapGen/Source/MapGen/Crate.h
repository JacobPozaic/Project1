#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Crate.generated.h"

UCLASS()
class MAPGEN_API ACrate : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY()
		UStaticMesh* crate_mesh_model;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* crate_mesh;
	ACrate();

protected:
	virtual void BeginPlay() override;
};
