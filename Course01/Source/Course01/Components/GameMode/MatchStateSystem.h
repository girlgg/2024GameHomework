#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Course01/Common/Round/MatchState.h"
#include "Course01/Common/Round/RoundState.h"
#include "GameFramework/GameMode.h"
#include "MatchStateSystem.generated.h"

class UMatchSettings;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartMatch);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPostRound);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPostMatch);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundInProgress);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaitingToStartRound);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeOut);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchScoresUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateRoundsWon);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COURSE01_API UMatchStateSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE ERoundState GetRoundState() const { return RoundState; }
	FORCEINLINE EMatchState GetMatchState() const { return MatchState; }
	FORCEINLINE bool GetIsMenu() const { return bMenu; }
	FORCEINLINE float GetTimeLeftToStartMatch() const { return TimeLeftToStartMatch; }
	FORCEINLINE bool IsGameOver() const { return MatchState == EMatchState::WaitingPostMatch; }
	FORCEINLINE bool IsRoundOver() const { return RoundState == ERoundState::PostRound; }
	FORCEINLINE int32 GetTeamAPoints() const { return TeamAPoints; }
	FORCEINLINE int32 GetTeamBPoints() const { return TeamBPoints; }
	FORCEINLINE float GetCurrentTime() const { return MatchTimeInSeconds; }

	UMatchStateSystem();

	void SetRoundState(ERoundState NewRoundState);
	void SetMatchState(EMatchState NewMatchState);

	void OnReset();
	void FlipRoundsWon();

	void AddPoints(int32 InTeamIdx, int32 InScore);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void StartGame();
	void StartMatchRound();
	void StartMatchTimer();

	void EndRound(int32 InWinnerTeam);
	bool AddRoundsWon(int32 InWinnerTeam);

	void ResetScores();
	void ResetLevel();

	void EndGame();
	
	UFUNCTION()
	void Update();
	UFUNCTION()
	void UpdateMatchTimer();
	UFUNCTION()
	void UpdateTimeOutCounter();

public:
	FOnStartMatch OnStartMatch;
	FOnPostMatch OnPostMatch;

	FMatchScoresUpdated MatchScoresUpdated;

	FOnWaitingToStartRound OnWaitingToStartRound;
	FOnRoundInProgress OnRoundInProgress;
	FOnPostRound OnPostRound;

	FOnTimeOut OnTimeOut;

	FUpdateRoundsWon UpdateRoundsWon;

protected:
	UPROPERTY()
	UMatchSettings* MatchSettings{nullptr};
	UPROPERTY()
	AGameMode* GameMode;

	FTimerHandle PreMatchTimerHandle;
	FTimerHandle TimeOutHandle;

	UPROPERTY(Replicated)
	float MatchTimeInSeconds{0.f};

	float PreMatchTimeInSeconds{0.f};
	UPROPERTY(Replicated)
	float TimeLeftToStartMatch{0.f};

	UFUNCTION()
	void OnRep_TeamAPoints();
	UPROPERTY(ReplicatedUsing=OnRep_TeamAPoints)
	int32 TeamAPoints{0};
	UFUNCTION()
	void OnRep_TeamBPoints();
	UPROPERTY(ReplicatedUsing=OnRep_TeamBPoints)
	int32 TeamBPoints{0};

	UFUNCTION()
	void OnRep_RoundsWonA();
	UPROPERTY(ReplicatedUsing=OnRep_RoundsWonA)
	int32 RoundsWonA{0};
	UFUNCTION()
	void OnRep_RoundsWonB();
	UPROPERTY(ReplicatedUsing=OnRep_RoundsWonB)
	int32 RoundsWonB{0};
	UPROPERTY(Replicated)
	int32 WinnerTeam{255};

	UFUNCTION()
	void OnRep_MatchState();
	UPROPERTY(ReplicatedUsing=OnRep_MatchState)
	EMatchState MatchState{EMatchState::WaitingToStart};

	UFUNCTION()
	void OnRep_RoundState();
	UPROPERTY(ReplicatedUsing=OnRep_RoundState)
	ERoundState RoundState{ERoundState::WaitingToStartRound};

	uint8 bMenu : 1{false};
};
