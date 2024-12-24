#include "BaseButtonWidget.h"

#include <comdef.h>

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBaseButtonWidget::OnButtonClicked()
{
	OnClicked.Broadcast();
}

void UBaseButtonWidget::OnButtonReleased()
{
	OnReleased.Broadcast();
}

void UBaseButtonWidget::OnButtonHovered()
{
	ButtonText->SetFont(HoveredTextFont);
	ButtonText->SetColorAndOpacity(HoveredColor);
}

void UBaseButtonWidget::OnButtonUnhovered()
{
	ButtonText->SetFont(DefaultTextFont);
	ButtonText->SetColorAndOpacity(DefaultColor);
}

void UBaseButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	ButtonText->SetJustification(InJustification);
	ButtonText->SetColorAndOpacity(DefaultColor);
	ButtonText->SetFont(DefaultTextFont);
	ButtonText->SetText(Text);

	BaseButton->SetBackgroundColor(ButtonBackgroundColor);
}

void UBaseButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BaseButton->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
	BaseButton->OnReleased.AddDynamic(this, &ThisClass::OnButtonReleased);
	BaseButton->OnHovered.AddDynamic(this, &ThisClass::OnButtonHovered);
	BaseButton->OnUnhovered.AddDynamic(this, &ThisClass::OnButtonUnhovered);
}
