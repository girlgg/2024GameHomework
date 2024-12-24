#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UIManagerComponent.generated.h"


class USpectateMatchWidget;
class ULoadingWidget;
class UCharacterSelectionWidget;
class UGameUIWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COURSE01_API UUIManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUIManagerComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void PossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	void CreateGameStateWidget();
	void CreateHUD();

	UFUNCTION()
	void RemoveHUD();

	void OnReset();

	UFUNCTION(Client, Reliable)
	void Client_ShowCharacterSelectionWidget();
	/* 下一回合重生的UI */
	void SpectateUntilNextRound();

	bool IsUsingMenus();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UGameUIWidget> GameUIClass;
	UPROPERTY(Transient)
	UGameUIWidget* GameUI{nullptr};

	UPROPERTY()
	UUserWidget* GameMenuUI;
	UPROPERTY()
	UUserWidget* SettingsUI;
	UPROPERTY()
	UUserWidget* GameStateUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ULoadingWidget> LoadingUIClass;
	UPROPERTY(Transient)
	ULoadingWidget* LoadingUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UCharacterSelectionWidget> CharacterSelectionUIClass;
	UPROPERTY(Transient)
	UCharacterSelectionWidget* CharacterSelectionUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<USpectateMatchWidget> SpectateMatchUIClass;
	UPROPERTY(Transient)
	USpectateMatchWidget* SpectateMatchUI;
};
