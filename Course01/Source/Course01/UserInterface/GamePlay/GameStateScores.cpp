#include "GameStateScores.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Course01/Components/GameMode/MatchSettings.h"
#include "Course01/Components/GameMode/MatchStateSystem.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

void UGameStateScores::NativeConstruct()
{
	Super::NativeConstruct();

	AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld());
	if (GameState)
	{
		if (UMatchSettings* MatchSettings = GameState->GetComponentByClass<UMatchSettings>())
		{
			PointsToWin = MatchSettings->PointsToWin;
		}
		MatchState = GameState->GetComponentByClass<UMatchStateSystem>();
		if (MatchState)
		{
			MatchState->MatchScoresUpdated.AddDynamic(this, &ThisClass::MatchScoresUpdated);
		}
	}
	TeamAScoreBar->SetPercent(0.f);
	TeamBScoreBar->SetPercent(0.f);
	TeamAScore->SetText(FText::FromString(TEXT("0")));
	TeamBScore->SetText(FText::FromString(TEXT("0")));
	MatchScoresUpdated();
}

void UGameStateScores::MatchScoresUpdated()
{
	if (!MatchState)
	{
		return;
	}
	int32 TeamAPoint = MatchState->GetTeamAPoints();
	int32 TeamBPoint = MatchState->GetTeamBPoints();

	TeamAScore->SetText(FText::AsNumber(TeamAPoint));
	TeamBScore->SetText(FText::AsNumber(TeamBPoint));

	TeamAScoreBar->SetPercent(TeamAPoint * 1.f / PointsToWin);
	TeamBScoreBar->SetPercent(TeamBPoint * 1.f / PointsToWin);
}
