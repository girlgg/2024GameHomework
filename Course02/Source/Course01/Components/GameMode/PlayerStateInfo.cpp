#include "PlayerStateInfo.h"

#include "Net/UnrealNetwork.h"


UPlayerStateInfo::UPlayerStateInfo()
{
}


void UPlayerStateInfo::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerStateInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerStateInfo, PlayerName);
}
