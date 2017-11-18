#include "Crate.h"

ACrate::ACrate() {
	USphereComponent* root = CreateDefaultSubobject<USphereComponent>(TEXT("Root Component"));		// create a sphere to use as the root of the map generator
	RootComponent = root;																			// set the sphere as the root component

	crate_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crate Mesh"));
	crate_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> crate_mesh_finder(TEXT("StaticMesh'/Game/Res/Meshes/crate.crate'"));
	crate_mesh_model = crate_mesh_finder.Object;

	crate_mesh->SetSimulatePhysics(true);
	crate_mesh->SetMassScale(NAME_None, 0.5f);

	crate_mesh->SetStaticMesh(crate_mesh_model);
}

void ACrate::BeginPlay() {
	Super::BeginPlay();
}
