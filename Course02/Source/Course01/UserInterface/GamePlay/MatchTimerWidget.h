#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "MatchTimerWidget.generated.h"

class UMatchSettings;
class UMatchStateSystem;
class UTextBlock;

UCLASS()
class COURSE01_API UMatchTimerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//~ UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End of UUserWidget interface

	UFUNCTION()
	void FinishTimer();

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TimeText;

	UPROPERTY(Transient)
	UMatchStateSystem* MatchState;
	UPROPERTY(Transient)
	UMatchSettings* MatchSettings;

	uint8 bAllowTick : 1{true};
};
