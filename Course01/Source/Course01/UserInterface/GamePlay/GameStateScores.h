#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GameStateScores.generated.h"

class UMatchStateSystem;
class UProgressBar;
class UTextBlock;

UCLASS()
class COURSE01_API UGameStateScores : public UUserWidget
{
	GENERATED_BODY()

protected:
	//~ UUserWidget interface
	virtual void NativeConstruct() override;
	//~ End of UUserWidget interface

	UFUNCTION()
	void MatchScoresUpdated();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	UTextBlock* TeamAScore;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	UProgressBar* TeamAScoreBar;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	UTextBlock* TeamBScore;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidget))
	UProgressBar* TeamBScoreBar;

	int32 PointsToWin{1};

	UPROPERTY(Transient)
	UMatchStateSystem* MatchState;
};
