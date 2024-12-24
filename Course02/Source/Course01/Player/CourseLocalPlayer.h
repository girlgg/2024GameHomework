#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "CourseLocalPlayer.generated.h"

UCLASS()
class COURSE01_API UCourseLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:
	UCourseLocalPlayer();

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, UCourseLocalPlayer* LocalPlayer,
	                                     APlayerController* PlayerController);
	FPlayerControllerSetDelegate OnPlayerControllerSet;
};
