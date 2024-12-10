#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.generated.h"

class AWeaponBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquipped, AWeaponBase*, WeaponEquipped);

UCLASS()
class COURSE01_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_CreateAndEquipLoadout(TSubclassOf<AWeaponBase> WeaponToSpawn,
	                                  USkeletalMeshComponent* FPPMesh,
	                                  USkeletalMeshComponent* TPPMesh);

protected:
	void CreateAndEquipLoadout(const TSubclassOf<AWeaponBase>& PrimaryWeaponToSpawn,
	                           USkeletalMeshComponent* FPPMesh,
	                           USkeletalMeshComponent* TPPMesh);

	void AttachWeapon(AWeaponBase* WeaponToSpawn,
	                  USkeletalMeshComponent* FPPMesh,
	                  USkeletalMeshComponent* TPPMesh);

	/* 展示武器准星 */
	void ShowWeaponCrosshair();

public:
	FOnWeaponEquipped OnWeaponEquipped;

protected:
	UFUNCTION()
	void OnRep_PrimaryWeapon();
	UPROPERTY(ReplicatedUsing=OnRep_PrimaryWeapon)
	AWeaponBase* PrimaryWeapon{nullptr};

public:
	UFUNCTION()
	void OnRep_CurrentWeapon();
	UPROPERTY(ReplicatedUsing=OnRep_CurrentWeapon)
	AWeaponBase* CurrentWeapon{nullptr};

protected:
	UPROPERTY(Transient)
	AWeaponBase* PreviousWeapon{nullptr};

	UPROPERTY(Transient)
	UUserWidget* CurrentCrosshairWidget;
};
