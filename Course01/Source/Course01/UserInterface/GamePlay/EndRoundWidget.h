#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "EndRoundWidget.generated.h"

UCLASS()
class COURSE01_API UEndRoundWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION(BlueprintCallable)
	int32 GetTeamAScore() const;
	UFUNCTION(BlueprintCallable)
	int32 GetTeamBScore() const;
};
