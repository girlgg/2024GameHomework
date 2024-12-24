﻿#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonUIExtensions.generated.h"

UCLASS()
class COURSE01_API UCommonUIExtensions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static FName SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason);

	static FName SuspendInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendReason);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static void ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken);

	static void ResumeInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendToken);

private:
	static int32 InputSuspensions;
};
