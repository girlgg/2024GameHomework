// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LuaGameMode.h"
#include "GameFramework/GameModeBase.h"
#include "Course01GameMode.generated.h"

UCLASS()
class ACourse01GameMode : public ALuaGameMode
{
	GENERATED_BODY()

public:
	ACourse01GameMode();

	virtual void BeginPlay() override;
};



