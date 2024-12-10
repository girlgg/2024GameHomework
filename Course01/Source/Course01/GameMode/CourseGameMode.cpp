#include "CourseGameMode.h"

#include "CourseGameStateBase.h"
#include "Course01/Components/GameMode/PlayerStateInfo.h"
#include "Course01/Components/Player/TeamComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

ACourseGameMode::ACourseGameMode()
{
	bDelayedStart = true;
	MinRespawnDelay = 5.f;
	MaxInactivePlayers = 6;
	bStartPlayersAsSpectators = true;
	bPauseable = false;
}

void ACourseGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (UPlayerStateInfo* PSI = NewPlayer->PlayerState->GetComponentByClass<UPlayerStateInfo>())
	{
		// FString Message = FString::Printf(TEXT("%s join the game"), *PSI->GetPlayerName());
		FString Message = FString::Format(TEXT("{0} join the game"), {PSI->GetPlayerName()});
		Server_SendChatMessage(Message);
	}
}

void ACourseGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (UPlayerStateInfo* PSI = Exiting->PlayerState->GetComponentByClass<UPlayerStateInfo>())
	{
		// FString Message = FString::Printf(TEXT("%s join the game"), *PSI->GetPlayerName());
		FString Message = FString::Format(TEXT("{0} left the game"), {PSI->GetPlayerName()});
		Server_SendChatMessage(Message);
	}
	if (ACourseGameStateBase* GS = GetGameState<ACourseGameStateBase>())
	{
		// TODO
		// GS->HandlePlayerLogout(Cast<APlayerController>(Exiting));
	}
}

void ACourseGameMode::SetMatchState(FName NewState)
{
	Super::SetMatchState(NewState);
	if (NewState == MatchState::WaitingToStart)
	{
	}
	else if (NewState == MatchState::InProgress)
	{
	}
	else if (NewState == MatchState::WaitingPostMatch)
	{
	}
	else
	{
	}
}

void ACourseGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	if (UTeamComponent* TeamComponent = NewPlayer->PlayerState->GetComponentByClass<UTeamComponent>())
	{
		if (GetPlayersByTeam(0).Num() <= GetPlayersByTeam(1).Num())
		{
			TeamComponent->SetTeam(0);
		}
		else
		{
			TeamComponent->SetTeam(1);
		}
	}
}

TArray<APlayerState*> ACourseGameMode::GetPlayersByTeam(int32 InTeamIdx)
{
	TArray<APlayerState*> TeamPlayers;
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (UTeamComponent* Teamcomponent = PS->GetComponentByClass<UTeamComponent>())
		{
			if (Teamcomponent->GetTeam() == InTeamIdx)
			{
				TeamPlayers.AddUnique(PS);
			}
		}
	}
	return TeamPlayers;
}

void ACourseGameMode::Server_SendChatMessage_Implementation(const FString& TextToSend)
{
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (APlayerController* PC = PS->GetPlayerController())
		{
		}
	}
}
