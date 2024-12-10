#include "ScoreCube.h"

#include "Components/BoxComponent.h"


AScoreCube::AScoreCube()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName(TEXT("BlockAll"));

	SetReplicates(true);
}

void AScoreCube::BeginPlay()
{
	Super::BeginPlay();
}
