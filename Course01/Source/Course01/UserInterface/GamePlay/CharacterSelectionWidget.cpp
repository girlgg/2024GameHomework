#include "CharacterSelectionWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Course01/Components/GameMode/MatchStateSystem.h"
#include "Course01/Components/Player/LoadoutComponent.h"
#include "Course01/GameMode/CourseGameStateBase.h"
#include "Course01/Player/CoursePlayerController.h"
#include "Course01/Weapon/WeaponBase.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void UCharacterSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 这个界面包含选择武器等，之后在这里初始化

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		if (ULoadoutComponent* Loadout = PC->GetComponentByClass<ULoadoutComponent>())
		{
			TSoftClassPtr<AWeaponBase> SoftPrimaryWeapon;
			Loadout->GetLoadout(SoftPrimaryWeapon);

			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				SoftPrimaryWeapon.ToSoftObjectPath(),
				FStreamableDelegate::CreateLambda([&]()
				{
					SelectedPrimaryWeapon = SoftPrimaryWeapon.Get();
				}));
		}
		PC->SetShowMouseCursor(true);
	}

	// TODO 设置武器Icon等

	UpdatePrimaryWeapon();

	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::PossessController);
	if (ACourseGameStateBase* CGSB = Cast<ACourseGameStateBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		if (UMatchStateSystem* MSS = CGSB->GetComponentByClass<UMatchStateSystem>())
		{
			MSS->OnPostRound.AddDynamic(this, &ThisClass::RemoveWidget);
			MSS->OnPostMatch.AddDynamic(this, &ThisClass::RemoveWidget);
		}
	}

	if (PC)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
		PC->SetViewTarget(PlayerStarts[0]);
	}

	// 初始化按钮
	InitButtons();
	// 绑定按钮事件
	PlayButton->OnReleased.AddDynamic(this, &ThisClass::PlayButton_OnReleased);
	PlayButton->OnHovered.AddDynamic(this, &ThisClass::PlayButton_OnHovered);
	PlayButton->OnUnhovered.AddDynamic(this, &ThisClass::PlayButton_OnUnHovered);
}

void UCharacterSelectionWidget::PossessController(APawn* OldPawn, APawn* NewPawn)
{
	RemoveWidget();
}

void UCharacterSelectionWidget::RemoveWidget()
{
	RemoveFromParent();
}

void UCharacterSelectionWidget::UpdatePrimaryWeapon()
{
	if (!PrimaryWeapons.IsValidIndex(SelectedPrimaryWeaponIdx))
	{
		return;
	}
	SelectedPrimaryWeapon = PrimaryWeapons[SelectedPrimaryWeaponIdx];
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (ULoadoutComponent* Loadout = PC->GetComponentByClass<ULoadoutComponent>())
		{
			Loadout->Server_SetPrimaryWeapon(SelectedPrimaryWeapon);
		}
	}
}

void UCharacterSelectionWidget::InitButtons()
{
	OnButtonHovered(PlayButton, PlayText);
}

void UCharacterSelectionWidget::OnButtonHovered(UButton* InButton, UTextBlock* Text)
{
	if (InButton)
	{
		InButton->SetBackgroundColor(ButtonHoveredColor);
	}
	if (Text)
	{
		Text->SetColorAndOpacity(ButtonTextHoveredColor);
	}
}

void UCharacterSelectionWidget::OnButtonUnhovered(UButton* InButton, UTextBlock* Text)
{
	InButton->SetBackgroundColor(ButtonUnHoveredColor);
	Text->SetColorAndOpacity(ButtonTextUnHoveredColor);
}

void UCharacterSelectionWidget::PlayButton_OnReleased()
{
	// 必须玩家鼠标在上面时松开才会开始
	if (!PlayButton->IsHovered())
	{
		return;
	}

	PlayButton->SetIsEnabled(false);
	if (ACoursePlayerController* APC = Cast<ACoursePlayerController>(GetOwningPlayer()))
	{
		APC->Spawn();
		APC->SetShowMouseCursor(false);
	}

	RemoveWidget();
}

void UCharacterSelectionWidget::PlayButton_OnHovered()
{
	OnButtonUnhovered(PlayButton, PlayText);
}

void UCharacterSelectionWidget::PlayButton_OnUnHovered()
{
	OnButtonHovered(PlayButton, PlayText);
}
