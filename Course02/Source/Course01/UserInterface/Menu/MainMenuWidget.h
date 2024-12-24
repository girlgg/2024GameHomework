#pragma once

#include "CoreMinimal.h"
#include "Course01/UserInterface/Common/CourseActivatableWidget.h"
#include "MainMenuWidget.generated.h"

class UWidgetSwitcher;

UCLASS()
class COURSE01_API UMainMenuWidget : public UCourseActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuWidgetSwitcher;
};
