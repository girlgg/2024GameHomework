#include "MatchSettings.h"


UMatchSettings::UMatchSettings()
{
	SetIsReplicatedByDefault(true);
}

void UMatchSettings::SetAllowFriendlyFire(bool bInAllowFriendlyFire)
{
	bAllowFriendlyFire = bInAllowFriendlyFire;
}

void UMatchSettings::SetPointsToWin(int32 InPointsToWin)
{
	PointsToWin = InPointsToWin;
}

void UMatchSettings::SetRoundTimeLimit(float InRoundTimeLimit)
{
	RoundTimeLimit = InRoundTimeLimit;
}

void UMatchSettings::SetAllowBots(bool bInAllowBots)
{
	bAllowBots = bInAllowBots;
}

void UMatchSettings::BeginPlay()
{
	Super::BeginPlay();
}
