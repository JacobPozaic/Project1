#include "Torch.h"

ATorch::ATorch() {
	USphereComponent* root = CreateDefaultSubobject<USphereComponent>(TEXT("Root Component"));		// create a sphere to use as the root of the map generator
	RootComponent = root;																			// set the sphere as the root component

	torch_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch Mesh"));
	torch_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> torch_mesh_finder(TEXT("StaticMesh'/Game/Res/Meshes/torch.torch'"));
	torch_mesh_model = torch_mesh_finder.Object;
	torch_mesh->SetStaticMesh(torch_mesh_model);
	FLightingChannels torch_channels = FLightingChannels();
	torch_channels.bChannel0 = 0;
	torch_mesh->LightingChannels = torch_channels;

	torch_light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Torch Light"));

	torch_light->AddWorldTransform(FTransform(FVector(0, -90, 400)));
	torch_light->AddWorldRotation(FQuat::MakeFromEuler(FVector(0, -60, -90)));

	torch_light->Intensity = 3;
	torch_light->LightColor = FColor(255, 127, 0, 255);
	torch_light->AttenuationRadius = 4096;
	torch_light->bUseInverseSquaredFalloff = false;
	torch_light->LightFalloffExponent = 2;
	torch_light->MinRoughness = 0.5f;

	torch_light->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);	// make it a child of the root component
}

void ATorch::BeginPlay() {
	Super::BeginPlay();
}

