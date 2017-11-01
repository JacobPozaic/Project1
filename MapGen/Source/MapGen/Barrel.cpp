#include "Barrel.h"

ABarrel::ABarrel() {
	USphereComponent* root = CreateDefaultSubobject<USphereComponent>(TEXT("Root Component"));		// create a sphere to use as the root of the map generator
	RootComponent = root;																			// set the sphere as the root component

	barrel_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Mesh"));
	barrel_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> barrel_mesh_finder(TEXT("StaticMesh'/Game/Res/Meshes/Barrel.Barrel'"));
	barrel_mesh_model = barrel_mesh_finder.Object;

	barrel_mesh->SetSimulatePhysics(true);
	barrel_mesh->SetMassScale(NAME_None, 0.2f);

	barrel_mesh->SetStaticMesh(barrel_mesh_model);
}

void ABarrel::BeginPlay() {
	Super::BeginPlay();
}
