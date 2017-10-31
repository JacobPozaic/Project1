#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Torch.generated.h"

UCLASS()
class MAPGEN_API ATorch : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY()
		UStaticMesh* torch_mesh_model;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* torch_mesh;
	UPROPERTY(EditAnywhere)
		UPointLightComponent* torch_light;
	ATorch();

protected:
	virtual void BeginPlay() override;
};
