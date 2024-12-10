#include "PlayerSpawn.h"

#include "Components/CapsuleComponent.h"

APlayerSpawn::APlayerSpawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool APlayerSpawn::CheckCanBeUsed() const
{
	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors, APawn::StaticClass());
	return OverlappingActors.IsEmpty();
}

void APlayerSpawn::BeginPlay()
{
	Super::BeginPlay();
}
