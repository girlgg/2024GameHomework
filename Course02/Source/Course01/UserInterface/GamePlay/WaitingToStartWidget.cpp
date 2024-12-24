#include "WaitingToStartWidget.h"

#include "Course01/Components/GameMode/MatchStateSystem.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

void UWaitingToStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AGameStateBase* GS = UGameplayStatics::GetGameState(this))
	{
		if (UMatchStateSystem* MSS = GS->GetComponentByClass<UMatchStateSystem>())
		{
			MSS->OnRoundInProgress.AddDynamic(this, &ThisClass::RemoveWidget);
		}
	}
}

void UWaitingToStartWidget::RemoveWidget()
{
	RemoveFromParent();
}

float UWaitingToStartWidget::GetTimeLeft() const
{
	if (AGameStateBase* GS = UGameplayStatics::GetGameState(this))
	{
		if (UMatchStateSystem* MSS = GS->GetComponentByClass<UMatchStateSystem>())
		{
			return MSS->GetTimeLeftToStartMatch();
		}
	}
	return 0.f;
}
