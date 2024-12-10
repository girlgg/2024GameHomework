#pragma once

#include "RoundState.generated.h"

UENUM(BlueprintType)
enum class ERoundState : uint8
{
	WaitingToStartRound,
	RoundInProgress,
	PostRound,
	Max
};
