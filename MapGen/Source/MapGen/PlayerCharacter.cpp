#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) {
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &APlayerCharacter::MoveUp);
	InputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	InputComponent->BindAxis("Look", this, &APlayerCharacter::AddControllerPitchInput);
	InputComponent->BindAction("ToggleFlying", IE_Pressed, this, &APlayerCharacter::ToggleFlying);
}

void APlayerCharacter::MoveForward(float distance) {
	if ((Controller != NULL) && (distance != 0.0f)) {
		// calculate forward direction for the players current rotation
		const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, distance);
	}
}

void APlayerCharacter::MoveRight(float distance) {
	if ((Controller != NULL) && (distance != 0.0f)) {
		// calculate direction right for the players current rotation
		const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, distance);
	}
}

void APlayerCharacter::MoveUp(float distance) {
	if ((Controller != NULL) && (distance != 0.0f)) {
		// direction upwards for the players current rotation
		AddMovementInput(FVector(0, 0, 1), distance);
	}
}

void APlayerCharacter::ToggleFlying() {
	if (GetCharacterMovement()->IsFlying()) {
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bSimGravityDisabled = false;
	} else {
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->BrakingDecelerationFlying = 1000.0f;
		GetCharacterMovement()->MaxFlySpeed = 2400;
		bSimGravityDisabled = true;
	}
}