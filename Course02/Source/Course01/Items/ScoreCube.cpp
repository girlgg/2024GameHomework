#include "ScoreCube.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

AScoreCube::AScoreCube()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName(TEXT("BlockAll"));
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetIsReplicated(true);

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube->SetupAttachment(BoxComponent);
	Cube->SetIsReplicated(true);

	SetReplicates(true);
}

void AScoreCube::Server_OnHit_Implementation(FVector Impulse)
{
	Multicast_OnHit(Impulse);
}

void AScoreCube::Multicast_OnHit_Implementation(FVector Impulse)
{
	++CurrentHit;
	if (CurrentHit >= HitTimes)
	{
		Destroy();
	}
	else
	{
		BoxComponent->AddImpulse(Impulse, NAME_None, true);
		SetActorScale3D(GetActorScale3D() * CubeScale);
	}
}

void AScoreCube::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
}

void AScoreCube::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AScoreCube, CubeBaseScore);
	DOREPLIFETIME(AScoreCube, CubeScale);
	DOREPLIFETIME(AScoreCube, HitTimes);
	DOREPLIFETIME(AScoreCube, bIsImport);
	DOREPLIFETIME(AScoreCube, CurrentHit);
}
