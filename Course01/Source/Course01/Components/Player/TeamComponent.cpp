#include "TeamComponent.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


UTeamComponent::UTeamComponent()
{
	SetIsReplicatedByDefault(true);
}

void UTeamComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTeamComponent, Team);
}

void UTeamComponent::SetTeam(int32 InTeamIdx)
{
	Team = InTeamIdx;
	OnRep_Team();
	OnTeamChanged.Broadcast();
}

void UTeamComponent::Server_SetTeam_Implementation(int32 InTeamIdx)
{
	SetTeam(InTeamIdx);

	GetOwner()->ForceNetUpdate();
}


void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTeamComponent::OnRep_Team()
{
	if (APlayerState* PS = UGameplayStatics::GetPlayerState(GetWorld(), 0))
	{
		if (UTeamComponent* TeamComp = PS->GetComponentByClass<UTeamComponent>())
		{
			TeamComp->OnTeamChanged.Broadcast();
		}
		else
		{
			OnTeamChanged.Broadcast();
		}
	}
}
