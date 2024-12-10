#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingToStartWidget.generated.h"

class UTextBlock;

UCLASS()
class COURSE01_API UWaitingToStartWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void RemoveWidget();

	UFUNCTION(BlueprintCallable)
	float GetTimeLeft() const;
};
