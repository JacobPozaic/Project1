#include "MapGenGameMode.h"

UStaticMesh* floor_mesh;
UStaticMesh* door_mesh;
UStaticMesh* wall_mesh;
AMapLayout* map_layout;

AMapGenGameMode::AMapGenGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	DefaultPawnClass = APlayerCharacter::StaticClass();

	static ConstructorHelpers::FObjectFinder<UStaticMesh>floor_mesh_finder(TEXT("StaticMesh'/Game/Res/Meshes/plane.plane'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>door_mesh_finder(TEXT("StaticMesh'/Game/Res/Meshes/door.door'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>wall_mesh_finder(TEXT("StaticMesh'/Game/Res/Meshes/wall.wall'"));

	floor_mesh = floor_mesh_finder.Object;
	door_mesh = door_mesh_finder.Object;
	wall_mesh = wall_mesh_finder.Object;
}

void AMapGenGameMode::StartPlay() {
	Super::StartPlay();

	/* Configure and spawn a map generator */
	FActorSpawnParameters spawn_info;
	FTransform* trans = new FTransform(FVector(0.0f, 0.0f, 0.0f));
	map_layout = (AMapLayout*) GetWorld()->SpawnActor(AMapLayout::StaticClass(), trans, spawn_info);
	map_layout->Init(floor_mesh, door_mesh, wall_mesh);
	map_layout->GenerateMap();
}