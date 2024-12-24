#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseButtonWidget.generated.h"

class UTextBlock;
class UButton;
class UCanvasPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClick);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleased);

UCLASS()
class COURSE01_API UBaseButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETextJustify::Type> InJustification{ETextJustify::Left};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor DefaultColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor HoveredColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateFontInfo DefaultTextFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateFontInfo HoveredTextFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ButtonBackgroundColor;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnClick OnClicked;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnReleased OnReleased;

protected:
	UFUNCTION()
	void OnButtonClicked();
	UFUNCTION()
	void OnButtonReleased();
	UFUNCTION()
	void OnButtonHovered();
	UFUNCTION()
	void OnButtonUnhovered();

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCanvasPanel* CanvasPanel;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BaseButton;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* ButtonText;
};
