#include "EndRoundWidget.h"

#include "Course01/Components/GameMode/MatchStateSystem.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

void UEndRoundWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
}

void UEndRoundWidget::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	RemoveFromParent();
}

int32 UEndRoundWidget::GetTeamAScore() const
{
	if (AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld()))
	{
		if (UMatchStateSystem* MatchState = GameState->GetComponentByClass<UMatchStateSystem>())
		{
			return MatchState->GetTeamAPoints();
		}
	}
	return 0;
}

int32 UEndRoundWidget::GetTeamBScore() const
{
	if (AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld()))
	{
		if (UMatchStateSystem* MatchState = GameState->GetComponentByClass<UMatchStateSystem>())
		{
			return MatchState->GetTeamBPoints();
		}
	}
	return 0;
}
