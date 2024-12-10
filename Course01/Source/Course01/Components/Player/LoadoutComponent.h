#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LoadoutComponent.generated.h"

class AWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COURSE01_API ULoadoutComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULoadoutComponent();

	//~ UActorComponent interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End of UActorComponent interface

	void GetLoadout(TSoftClassPtr<AWeaponBase>& OutPrimaryWeapon);

	TSubclassOf<AWeaponBase> GetPrimaryWeaponClass();

	UFUNCTION(Server, Reliable)
	void Server_SetPrimaryWeapon(TSubclassOf<AWeaponBase> InPrimaryWeapon);

protected:
	virtual void BeginPlay() override;

	void SetPrimaryWeapon(TSubclassOf<AWeaponBase> InPrimaryWeapon);

	UPROPERTY(Replicated)
	TSubclassOf<AWeaponBase> PrimaryWeapon{nullptr};
};
