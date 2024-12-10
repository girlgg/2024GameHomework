#pragma once

#include "CoreMinimal.h"
#include "LuaActor.h"
#include "WeaponBase.generated.h"

class AProjectileBase;
class UWeaponCrosshairWidget;
class UInputMappingContext;
class ACoursePlayer;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAmmoChanged);

UCLASS()
class COURSE01_API AWeaponBase : public ALuaActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

	void Shoot(const FVector& FireLocation);

	void ReloadWeapon(int32 AddedAmmo);
	FORCEINLINE int32 GetActualAmmo() const { return ActualAmmo; }

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_SetActualAmmo(int32 InActualAmmo);

	UFUNCTION(Server, Reliable)
	void Server_SpawnProjectile(const FVector& FireLocation);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnProjectile(const FVector& FireLocation);
	void SpawnProjectile(const FVector& FireLocation);

public:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Properties")
	USkeletalMeshComponent* WeaponMeshFP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Properties")
	USkeletalMeshComponent* WeaponMeshTP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Properties")
	TSoftClassPtr<UWeaponCrosshairWidget> CrosshairWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Properties")
	TSubclassOf<AProjectileBase> ProjectileType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Settings | Animation")
	TSubclassOf<UAnimInstance> FirstPerson_WeaponAnims;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Settings | Animation")
	TSubclassOf<UAnimInstance> ThirdPerson_WeaponAnims;

	FAmmoChanged AmmoChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Settings | FirstPerson")
	FTransform FPPWeaponOffsetTransform;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Settings | ThirdPerson")
	FTransform TPPWeaponOffsetTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Settings | Ammo")
	int32 AmmoPerMag{30};

	UFUNCTION()
	void OnRep_ActualAmmo();
	UPROPERTY(ReplicatedUsing=OnRep_ActualAmmo)
	int32 ActualAmmo{5};
};
