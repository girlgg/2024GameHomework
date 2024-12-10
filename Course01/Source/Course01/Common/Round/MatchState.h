#pragma once

#include "MatchState.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	WaitingToStart,
	InProgress,
	WaitingPostMatch,
	Max
};