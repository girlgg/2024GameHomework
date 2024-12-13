#include "MatchStateSystem.h"

#include "MatchSettings.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


UMatchStateSystem::UMatchStateSystem()
{
	SetIsReplicatedByDefault(true);
}

void UMatchStateSystem::SetRoundState(ERoundState NewRoundState)
{
	RoundState = NewRoundState;
	OnRep_RoundState();

	GetOwner()->ForceNetUpdate();
}

void UMatchStateSystem::SetMatchState(EMatchState NewMatchState)
{
	if (GetOwner()->HasAuthority())
	{
		MatchState = NewMatchState;
		OnRep_MatchState();

		if (AGameMode* GM = Cast<AGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			switch (MatchState)
			{
			case EMatchState::WaitingToStart:
				break;
			case EMatchState::InProgress:
				GM->StartMatch();
				break;
			case EMatchState::WaitingPostMatch:
				GM->EndMatch();
				break;
			case EMatchState::Max:
				break;
			}
		}

		GetOwner()->ForceNetUpdate();
	}
}

void UMatchStateSystem::OnReset()
{
	ResetScores();
	StartGame();
}

void UMatchStateSystem::FlipRoundsWon()
{
	int32 Tmp = RoundsWonA;
	RoundsWonA = RoundsWonB;
	OnRep_RoundsWonA();
	RoundsWonB = Tmp;
	OnRep_RoundsWonB();
}

void UMatchStateSystem::AddPoints(int32 InTeamIdx, int32 InScore)
{
	if (InTeamIdx == 0)
	{
		TeamAPoints = TeamAPoints + InScore;
		OnRep_TeamAPoints();
	}
	else if (InTeamIdx == 1)
	{
		TeamBPoints = TeamBPoints + InScore;
		OnRep_TeamBPoints();
	}
	AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld());
	if (!GameState)
	{
		return;
	}
	if (!MatchSettings)
	{
		return;
	}
	if (TeamAPoints >= MatchSettings->PointsToWin)
	{
		EndRound(0);
	}
	else if (TeamBPoints >= MatchSettings->PointsToWin)
	{
		EndRound(1);
	}
}

void UMatchStateSystem::StartMatchRound()
{
	if (GetOwner()->HasAuthority())
	{
		StartMatchTimer();
		SetRoundState(ERoundState::RoundInProgress);
		SetMatchState(EMatchState::InProgress);
		if (GameMode)
		{
			GameMode->StartMatch();
		}
	}
}

void UMatchStateSystem::StartMatchTimer()
{
	if (MatchSettings->bMatchWithTimeLimit)
	{
		GetWorld()->GetTimerManager().SetTimer(TimeOutHandle, [this]()
		{
			OnTimeOut.Broadcast();
		}, MatchSettings->RoundTimeLimit, false);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateTimeOutCounter,
		                                       1.f, true);
		UpdateMatchTimer();
		UpdateTimeOutCounter();
	}
	else
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateMatchTimer,
		                                       1.f, true);
		PreMatchTimeInSeconds = GetWorld()->GetTimeSeconds();
	}
}

void UMatchStateSystem::EndRound(int32 InWinnerTeam)
{
	if (GetRoundState() != ERoundState::RoundInProgress)
	{
		return;
	}
	WinnerTeam = InWinnerTeam;
	GetOwner()->ForceNetUpdate();
	float PostRoundTime = 0.f;
	if (MatchSettings)
	{
		PostRoundTime = MatchSettings->GetPostRoundTime();
	}
	if (AddRoundsWon(InWinnerTeam))
	{
		SetRoundState(ERoundState::PostRound);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::EndGame, PostRoundTime, false);
	}
	else
	{
		SetRoundState(ERoundState::PostRound);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::ResetLevel, PostRoundTime, false);
	}
}

bool UMatchStateSystem::AddRoundsWon(int32 InWinnerTeam)
{
	if (InWinnerTeam == 0)
	{
		RoundsWonA += 1;
		OnRep_RoundsWonA();
	}
	else if (InWinnerTeam == 1)
	{
		RoundsWonB += 1;
		OnRep_RoundsWonB();
	}
	if (MatchSettings)
	{
		int32 RoundsToWin = MatchSettings->GetRoundsToWin();
		if (RoundsWonA >= RoundsToWin || RoundsWonB >= RoundsToWin)
		{
			return true;
		}
	}
	return false;
}

void UMatchStateSystem::ResetScores()
{
	TeamAPoints = 0;
	OnRep_TeamAPoints();
	TeamBPoints = 0;
	OnRep_TeamBPoints();
	GetOwner()->ForceNetUpdate();
}

void UMatchStateSystem::ResetLevel()
{
}

void UMatchStateSystem::EndGame()
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	SetMatchState(EMatchState::WaitingPostMatch);
	GameMode->EndMatch();

	// TODO 断开会话
}

void UMatchStateSystem::Update()
{
	TimeLeftToStartMatch = GetWorld()->GetTimerManager().GetTimerRemaining(PreMatchTimerHandle);
	GetOwner()->ForceNetUpdate();
}

void UMatchStateSystem::UpdateMatchTimer()
{
	MatchTimeInSeconds = GetWorld()->GetTimeSeconds() - PreMatchTimeInSeconds;
}

void UMatchStateSystem::UpdateTimeOutCounter()
{
	MatchTimeInSeconds = GetWorld()->GetTimerManager().GetTimerRemaining(TimeOutHandle);
	if (MatchTimeInSeconds < .0001f)
	{
		// 本轮结束
		if (TeamAPoints >= TeamBPoints)
		{
			EndRound(0);
		}
		else
		{
			EndRound(1);
		}
	}
}

void UMatchStateSystem::BeginPlay()
{
	Super::BeginPlay();
	StartGame();
}

void UMatchStateSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMatchStateSystem, MatchTimeInSeconds);
	DOREPLIFETIME(UMatchStateSystem, TimeLeftToStartMatch);
	DOREPLIFETIME(UMatchStateSystem, TeamAPoints);
	DOREPLIFETIME(UMatchStateSystem, TeamBPoints);
	DOREPLIFETIME(UMatchStateSystem, RoundsWonA);
	DOREPLIFETIME(UMatchStateSystem, RoundsWonB);
	DOREPLIFETIME(UMatchStateSystem, MatchState);
	DOREPLIFETIME(UMatchStateSystem, RoundState);
}

void UMatchStateSystem::StartGame()
{
	if (GetOwner()->HasAuthority())
	{
		MatchSettings = UGameplayStatics::GetGameState(GetWorld())->GetComponentByClass<UMatchSettings>();
		if (AGameMode* GM = Cast<AGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GameMode = GM;
		}
		SetRoundState(ERoundState::WaitingToStartRound);
		SetMatchState(EMatchState::WaitingToStart);

		GetWorld()->GetTimerManager().SetTimer(PreMatchTimerHandle, this, &ThisClass::StartMatchRound,
		                                       MatchSettings->PreMatchTime, false);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::Update, .1f, true);
		Update();
	}
}

void UMatchStateSystem::OnRep_TeamAPoints()
{
	MatchScoresUpdated.Broadcast();
}

void UMatchStateSystem::OnRep_TeamBPoints()
{
	MatchScoresUpdated.Broadcast();
}

void UMatchStateSystem::OnRep_RoundsWonA()
{
	UpdateRoundsWon.Broadcast();
}

void UMatchStateSystem::OnRep_RoundsWonB()
{
	UpdateRoundsWon.Broadcast();
}

void UMatchStateSystem::OnRep_MatchState()
{
	switch (MatchState)
	{
	case EMatchState::WaitingToStart:
		break;
	case EMatchState::InProgress:
		OnStartMatch.Broadcast();
		break;
	case EMatchState::WaitingPostMatch:
		OnPostMatch.Broadcast();
		break;
	case EMatchState::Max:
		break;
	}
}

void UMatchStateSystem::OnRep_RoundState()
{
	switch (RoundState)
	{
	case ERoundState::WaitingToStartRound:
		OnWaitingToStartRound.Broadcast();
		break;
	case ERoundState::RoundInProgress:
		OnRoundInProgress.Broadcast();
		break;
	case ERoundState::PostRound:
		OnPostRound.Broadcast();
		break;
	case ERoundState::Max:
		break;
	}
}
