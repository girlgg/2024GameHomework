#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoursePlayer.generated.h"

class UHealthComponent;
class UInteractSystemComponent;
class UInventoryComponent;
class UPawnInfoComponent;
class UInputMappingContext;
class USpringArmComponent;
class AWeaponBase;
struct FInputActionValue;
class UCapsuleComponent;
class UCameraComponent;
class UInputAction;

UCLASS()
class COURSE01_API ACoursePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	FORCEINLINE FRotator GetFacingRotation() const {return ControlRotation;}
	
	ACoursePlayer();

	//~ Actor interface
	virtual void Tick(float DeltaTime) override;
	//~ End of Actor interface

	//~ Character interface
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BecomeViewTarget(APlayerController* PC) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End of Character interface

	//~=============================================================================
	// 处理回合事件

	/* 回合结束 */
	UFUNCTION()
	void OnPostRound();
	/* 添加玩家输入上下文映射 */
	UFUNCTION()
	void AddPlayerInputs();

	void GetPawnMeshes(USkeletalMeshComponent*& FPP, USkeletalMeshComponent*& TPP);

protected:
	void SetPawnTeam();
	
	UFUNCTION()
	void OnWeaponEquipped(AWeaponBase* WeaponEquipped);

	void SetWeaponAnimBlueprints(const AWeaponBase* WeaponEquipped) const;

	//~=============================================================================
	// 处理输入事件
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Fire(const FInputActionValue& Value);
	void StopFire(const FInputActionValue& Value);

	void EnableDamage();
	bool CanShoot();
	bool TryToShoot();

	// TODO 开始前提供UI，给玩家选择武器
	/* TODO 开始游戏前给玩家提供各类Perk */
	void ApplySelectedPerk();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPawnInfoComponent* PawnInfoComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventorySystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInteractSystemComponent* InteractSystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY()
	AWeaponBase* OverlappingWeapon;

	UPROPERTY(Replicated)
	FRotator ControlRotation;

	FTimerHandle AutoShootTimerHandle;

	UFUNCTION()
	void OnRep_PawnTeam();
	UPROPERTY(ReplicatedUsing=OnRep_PawnTeam)
	int32 PawnTeam{255};

	uint8 bCanShoot : 1{true};
	uint8 bWantToFire : 1{false};
};
