#include "ProjectileBase.h"

#include "Components/BoxComponent.h"
#include "Course01/Components/GameMode/MatchStateSystem.h"
#include "Course01/Components/Player/TeamComponent.h"
#include "Course01/Items/ScoreCube.h"
#include "Course01/Player/CoursePlayerStateBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetBoxExtent(FVector(2.f, 2.f, 2.f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionBox);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void AProjectileBase::OnLaunch(const FVector& Velocity)
{
	Multicast_SetVelocity(Velocity);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void AProjectileBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}
	AActor* OwnerWeapon = GetOwner();
	if (!OwnerWeapon)
	{
		return;
	}
	ACharacter* OwnerPlayer = Cast<ACharacter>(OwnerWeapon->GetOwner());
	if (!OwnerPlayer)
	{
		return;
	}
	AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld());
	if (!GameState)
	{
		return;
	}
	UMatchStateSystem* MatchState = GameState->GetComponentByClass<UMatchStateSystem>();
	if (!MatchState)
	{
		return;
	}
	if (AScoreCube* ScoreCube = Cast<AScoreCube>(OtherActor))
	{
		if (ACoursePlayerStateBase* PlayerState = Cast<ACoursePlayerStateBase>(OwnerPlayer->GetPlayerState()))
		{
			if (UTeamComponent* Team = PlayerState->GetComponentByClass<UTeamComponent>())
			{
				int32 TeamIdx = Team->GetTeam();
				MatchState->AddPoints(TeamIdx);
				PlayerState->AddScoreToPlayer(50);
			}
		}
		ScoreCube->Destroy();
		Destroy();
	}
}

void AProjectileBase::Multicast_SetVelocity_Implementation(const FVector& Velocity)
{
	ProjectileMovement->SetVelocityInLocalSpace(Velocity);
}
