#include "MatchTimerWidget.h"

#include "Components/TextBlock.h"
#include "Course01/Components/GameMode/MatchStateSystem.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

void UMatchTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TimeText->SetText(FText());
	if (AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld()))
	{
		MatchState = GameState->GetComponentByClass<UMatchStateSystem>();
		MatchSettings = GameState->GetComponentByClass<UMatchSettings>();
	}
	bAllowTick = true;
	if (MatchState)
	{
		MatchState->OnPostMatch.AddDynamic(this, &ThisClass::FinishTimer);
		MatchState->OnPostRound.AddDynamic(this, &ThisClass::FinishTimer);
	}
}

void UMatchTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bAllowTick)
	{
		return;
	}
	if (MatchState && MatchState->GetRoundState() == ERoundState::RoundInProgress)
	{
		float CurrentTime = MatchState->GetCurrentMatchTime();
		if (CurrentTime > 0.f)
		{
			SetVisibility(ESlateVisibility::HitTestInvisible);
			int32 CurrentTimeInt = CurrentTime;
			FString ReadableTimeString = FString::Printf(TEXT("%02d:%02d"), CurrentTimeInt / 60, CurrentTimeInt % 60);
			FText ReadableTimeText = FText::FromString(ReadableTimeString);
			TimeText->SetText(ReadableTimeText);
		}
		else
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMatchTimerWidget::FinishTimer()
{
	bAllowTick = false;
}
