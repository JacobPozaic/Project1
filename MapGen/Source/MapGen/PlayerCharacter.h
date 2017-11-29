#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MAPGEN_API APlayerCharacter : public ACharacter {
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MoveForward(float distance);
	UFUNCTION()
	void MoveRight(float distance);
	UFUNCTION()
	void MoveUp(float distance);
	UFUNCTION()
	void ToggleFlying();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
};
