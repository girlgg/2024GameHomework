#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnSystemComponent.generated.h"

class APlayerSpawn;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSpawned, APawn*, InPawn);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COURSE01_API USpawnSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpawnSystemComponent();

	UFUNCTION(Server, Reliable)
	void Server_SpawnPlayerWithTeam(APlayerController* InPlayerController);

protected:
	virtual void BeginPlay() override;

	APlayerSpawn* ChooseSpawn(int32 InTeamIdx);
	void SpawnPlayer(APlayerSpawn* InSpawn, APlayerController* InPlayerController);

public:
	FOnPlayerSpawned OnPlayerSpawned;

protected:
	TArray<APlayerSpawn*> TeamASpawn;
	TArray<APlayerSpawn*> TeamBSpawn;
	TArray<APlayerSpawn*> Spawns;
};
