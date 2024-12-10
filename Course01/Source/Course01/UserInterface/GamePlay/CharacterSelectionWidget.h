#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionWidget.generated.h"

class AWeaponBase;
class UTextBlock;
class UButton;

UCLASS()
class COURSE01_API UCharacterSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//~ UUserWidget interface
	virtual void NativeConstruct() override;
	//~ End of UUserWidget interface
protected:
	UFUNCTION()
	void PossessController(APawn* OldPawn, APawn* NewPawn);
	UFUNCTION()
	void RemoveWidget();

	void UpdatePrimaryWeapon();

	//~=============================================================================
	// 按钮事件
	void InitButtons();
	UFUNCTION(BlueprintCallable)
	void OnButtonHovered(UButton* InButton, UTextBlock* Text);
	UFUNCTION(BlueprintCallable)
	void OnButtonUnhovered(UButton* InButton, UTextBlock* Text);

	UFUNCTION()
	void PlayButton_OnReleased();
	UFUNCTION()
	void PlayButton_OnHovered();
	UFUNCTION()
	void PlayButton_OnUnHovered();

	UPROPERTY(meta=(BindWidget))
	UButton* PlayButton;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor ButtonHoveredColor{1., 1., 1., .9};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor ButtonUnHoveredColor{.0, .0, .0, .8};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateColor ButtonTextHoveredColor{FLinearColor(.2f, .2f, .2f, 1.0f)};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateColor ButtonTextUnHoveredColor{FLinearColor(.8f, .8f, .8f, .9f)};

	// TODO 枪械配置太多，配表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AWeaponBase>> PrimaryWeapons;

	TSubclassOf<AWeaponBase> SelectedPrimaryWeapon;
	int32 SelectedPrimaryWeaponIdx{0};
};
