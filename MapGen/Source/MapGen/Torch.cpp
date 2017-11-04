#include "Torch.h"

ATorch::ATorch() {
	USphereComponent* root = CreateDefaultSubobject<USphereComponent>(TEXT("Root Component"));		// create a sphere to use as the root of the map generator
	RootComponent = root;																			// set the sphere as the root component

	torch_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch Mesh"));
	torch_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> torch_mesh_finder(TEXT("StaticMesh'/Game/Res/Meshes/torch.torch'"));
	torch_mesh_model = torch_mesh_finder.Object;
	
	torch_mesh->SetStaticMesh(torch_mesh_model);
	torch_mesh->CastShadow = 0;

	torch_light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Torch Light"));

	torch_light->AddWorldTransform(FTransform(FVector(0, -50, 450)));
	torch_light->AddWorldRotation(FQuat::MakeFromEuler(FVector(0, -60, -90)));

	torch_light->Intensity = 4;
	torch_light->LightColor = FColor(255, 127, 0, 255);
	torch_light->AttenuationRadius = 2048;
	torch_light->bUseInverseSquaredFalloff = false;
	torch_light->LightFalloffExponent = 3;
	torch_light->MinRoughness = 0.5f;

	torch_light->Mobility = EComponentMobility::Stationary;

	torch_light->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> torch_fire_finder(TEXT("ParticleSystem'/Game/Res/Particles/TorchFire.TorchFire'"));
	torch_fire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Torch Fire"));
	torch_fire->Template = torch_fire_finder.Object;
	torch_fire->bAutoActivate = true;
	torch_fire->SetHiddenInGame(false);
}

void ATorch::BeginPlay() {
	Super::BeginPlay();
	torch_fire->AttachTo(torch_light);
}

