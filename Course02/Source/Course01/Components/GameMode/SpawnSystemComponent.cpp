#include "SpawnSystemComponent.h"

#include "Course01/Components/Player/TeamComponent.h"
#include "Course01/Player/PlayerSpawn.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"


USpawnSystemComponent::USpawnSystemComponent()
{
}

void USpawnSystemComponent::Server_SpawnPlayerWithTeam_Implementation(APlayerController* InPlayerController)
{
	if (InPlayerController->GetPawn())
	{
		return;
	}
	UTeamComponent* TeamComponent = InPlayerController->PlayerState->GetComponentByClass<UTeamComponent>();
	if (TeamComponent)
	{
		int32 TeamIdx = TeamComponent->GetTeam();
		APlayerSpawn* PlayerSpawn = ChooseSpawn(TeamIdx);
		SpawnPlayer(PlayerSpawn, InPlayerController);
	}
}


void USpawnSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
	{
		TArray<AActor*> SpawnActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerSpawn::StaticClass(), SpawnActors);
		for (AActor* Spawn : SpawnActors)
		{
			if (APlayerSpawn* PlayerSpawn = Cast<APlayerSpawn>(Spawn))
			{
				Spawns.Add(PlayerSpawn);
				if (PlayerSpawn->TeamIdx == 0)
				{
					TeamASpawn.Add(PlayerSpawn);
				}
				else
				{
					TeamBSpawn.Add(PlayerSpawn);
				}
			}
		}
	}
}

APlayerSpawn* USpawnSystemComponent::ChooseSpawn(int32 InTeamIdx)
{
	TArray<APlayerSpawn*> SelectedSpawns;
	TArray<APlayerSpawn*>& TeamSpawn = InTeamIdx == 0 ? TeamASpawn : TeamBSpawn;

	// 生成逻辑：出生点可用，并且附近无玩家，在这样的出生点中随机选择一个
	// 否则所有出生点随机选一个
	for (APlayerSpawn* CurrentSpawn : TeamSpawn)
	{
		if (CurrentSpawn->IsActive() && CurrentSpawn->CheckCanBeUsed())
		{
			if (AGameStateBase* GSB = UGameplayStatics::GetGameState(GetWorld()))
			{
				const TArray<TObjectPtr<APlayerState>>& PlayerArray = GSB->PlayerArray;
				bool bNearAnotherPlayer = false;
				for (const TObjectPtr<APlayerState>& PlayerState : PlayerArray)
				{
					if (PlayerState->GetPawn())
					{
						FVector ActorLocation = PlayerState->GetPawn()->GetActorLocation();
						FVector SpawnLocation = CurrentSpawn->GetActorLocation();
						if ((ActorLocation - SpawnLocation).Length() < 1200.f)
						{
							bNearAnotherPlayer = true;
						}
					}
				}
				if (!bNearAnotherPlayer)
				{
					SelectedSpawns.Add(CurrentSpawn);
				}
			}
		}
	}
	if (SelectedSpawns.IsEmpty())
	{
		return TeamSpawn[FMath::RandRange(0, TeamSpawn.Num() - 1)];
	}
	return SelectedSpawns[FMath::RandRange(0, SelectedSpawns.Num() - 1)];
}

void USpawnSystemComponent::SpawnPlayer(APlayerSpawn* InSpawn, APlayerController* InPlayerController)
{
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(GetWorld()))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParameters.Owner = InPlayerController;
		APawn* Player =
			Cast<APawn>(GetWorld()->SpawnActor(GM->DefaultPawnClass, &InSpawn->GetActorTransform(), SpawnParameters));
		if (Player)
		{
			InPlayerController->Possess(Player);
		}
	}
}
