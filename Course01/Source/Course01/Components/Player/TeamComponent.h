// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeamComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeamChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COURSE01_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTeamComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_SetTeam(int32 InTeamIdx);
	void SetTeam(int32 InTeamIdx);

	FORCEINLINE int32 GetTeam() const { return Team; }

protected:
	virtual void BeginPlay() override;

public:
	FOnTeamChanged OnTeamChanged;

protected:
	UFUNCTION()
	void OnRep_Team();
	UPROPERTY(ReplicatedUsing=OnRep_Team)
	int32 Team{255};
};
