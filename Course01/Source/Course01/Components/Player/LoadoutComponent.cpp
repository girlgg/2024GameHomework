#include "LoadoutComponent.h"

#include "Course01/Weapon/WeaponBase.h"
#include "Net/UnrealNetwork.h"


ULoadoutComponent::ULoadoutComponent()
{
	SetIsReplicatedByDefault(true);
}

void ULoadoutComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULoadoutComponent, PrimaryWeapon);
}

void ULoadoutComponent::GetLoadout(TSoftClassPtr<AWeaponBase>& OutPrimaryWeapon)
{
	OutPrimaryWeapon = GetPrimaryWeaponClass();
}

TSubclassOf<AWeaponBase> ULoadoutComponent::GetPrimaryWeaponClass()
{
	return PrimaryWeapon;
}

void ULoadoutComponent::Server_SetPrimaryWeapon_Implementation(TSubclassOf<AWeaponBase> InPrimaryWeapon)
{
	SetPrimaryWeapon(InPrimaryWeapon);
	GetOwner()->ForceNetUpdate();
}

void ULoadoutComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULoadoutComponent::SetPrimaryWeapon(TSubclassOf<AWeaponBase> InPrimaryWeapon)
{
	PrimaryWeapon = InPrimaryWeapon;
}
