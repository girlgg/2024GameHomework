#include "UIManagerComponent.h"

#include "MatchSettings.h"
#include "MatchStateSystem.h"
#include "Course01/GameMode/CourseGameStateBase.h"
#include "Course01/Player/CoursePlayerController.h"
#include "Course01/UserInterface/GamePlay/CharacterSelectionWidget.h"
#include "Course01/UserInterface/GamePlay/GameUIWidget.h"
#include "Course01/UserInterface/GamePlay/LoadingWidget.h"
#include "Course01/UserInterface/GamePlay/SpectateMatchWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


UUIManagerComponent::UUIManagerComponent()
{
}

void UUIManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UKismetSystemLibrary::IsDedicatedServer(GetWorld()))
	{
		return;
	}
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		PC->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::PossessedPawnChanged);
	}
	AGameStateBase* GS = UGameplayStatics::GetGameState(GetWorld());
	if (UMatchStateSystem* MSS = GS->GetComponentByClass<UMatchStateSystem>())
	{
		MSS->OnPostMatch.AddDynamic(this, &ThisClass::RemoveHUD);
		MSS->OnPostRound.AddDynamic(this, &ThisClass::RemoveHUD);
		if (!MSS->GetIsMenu())
		{
			if (UMatchSettings* MS = GS->GetComponentByClass<UMatchSettings>())
			{
				if (MSS->GetRoundState() == ERoundState::WaitingToStartRound || MS->GetAllowRespawn())
				{
					Client_ShowCharacterSelectionWidget();
				}
				else
				{
					SpectateUntilNextRound();
				}
			}
		}
	}
}

void UUIManagerComponent::PossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (Cast<ACharacter>(NewPawn))
	{
		CreateHUD();
	}
	else
	{
		RemoveHUD();
	}
}

void UUIManagerComponent::CreateGameStateWidget()
{
	if (IsValid(GameStateUI))
	{
		GameStateUI->RemoveFromParent();
		GameStateUI = nullptr;
	}
	if (ACourseGameStateBase* CGSB = Cast<ACourseGameStateBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		TSoftClassPtr<UUserWidget> GameModeWidgetClass, ScoresBoardClass, EndGmeScoresClass;
		CGSB->GetGameModeWidgets(GameModeWidgetClass, ScoresBoardClass, EndGmeScoresClass);
		TSubclassOf<UUserWidget> GameModeWidget = GameModeWidgetClass.LoadSynchronous();
		if (IsValid(GameModeWidget))
		{
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				GameStateUI = CreateWidget(PC, GameModeWidget);
				if (GameStateUI)
				{
					GameStateUI->AddToViewport();
				}
			}
		}
	}
}

void UUIManagerComponent::CreateHUD()
{
	if (IsValid(GameUI))
	{
		GameUI->RemoveFromParent();
		GameUI = nullptr;
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (UClass* LoadedClass = GameUIClass.LoadSynchronous())
	{
		GameUI = CreateWidget<UGameUIWidget>(PC, LoadedClass);
		if (GameUI)
		{
			GameUI->AddToViewport();
		}
	}
	CreateGameStateWidget();
	if (!IsUsingMenus())
	{
		PC->SetShowMouseCursor(false);
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}

void UUIManagerComponent::RemoveHUD()
{
	if (GameUI)
	{
		GameUI->RemoveFromParent();
		GameUI = nullptr;
	}
}

void UUIManagerComponent::OnReset()
{
	RemoveHUD();
	Client_ShowCharacterSelectionWidget();
}

void UUIManagerComponent::Client_ShowCharacterSelectionWidget_Implementation()
{
	if (IsValid(CharacterSelectionUI))
	{
		CharacterSelectionUI->RemoveFromParent();
		CharacterSelectionUI = nullptr;
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->DisableInput(PC);
		LoadingUI = CreateWidget<ULoadingWidget>(PC, LoadingUIClass);
		LoadingUI->AddToViewport();
	}

	// 测试发现这个Widget可能很大，所以尝试异步加载，加载时显示Loading...的UI
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(
		CharacterSelectionUIClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([&]()
		{
			if (IsValid(LoadingUI))
			{
				LoadingUI->RemoveFromParent();
				LoadingUI = nullptr;
			}
			TSubclassOf<UCharacterSelectionWidget> CSWClass = CharacterSelectionUIClass.Get();
			if (IsValid(CSWClass))
			{
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					CharacterSelectionUI = CreateWidget<UCharacterSelectionWidget>(PC, CSWClass);
					PC->EnableInput(PC);
					if (CharacterSelectionUI)
					{
						CharacterSelectionUI->AddToViewport();
						FInputModeGameAndUI InputMode;
						InputMode.SetWidgetToFocus(CharacterSelectionUI->TakeWidget());
						PC->SetInputMode(InputMode);
						PC->SetShowMouseCursor(true);
					}
				}
			}
		}));
}

void UUIManagerComponent::SpectateUntilNextRound()
{
	if (ACoursePlayerController* CPC = Cast<ACoursePlayerController>(GetOwner()))
	{
		if (UClass* LoadedClass = SpectateMatchUIClass.LoadSynchronous())
		{
			SpectateMatchUI = CreateWidget<USpectateMatchWidget>(CPC, LoadedClass);
			SpectateMatchUI->AddToViewport();
		}
	}
}

bool UUIManagerComponent::IsUsingMenus()
{
	if (IsValid(GameMenuUI))
	{
		return GameMenuUI->IsVisible();
	}
	if (IsValid(SettingsUI))
	{
		return SettingsUI->IsVisible();
	}
	return false;
}
