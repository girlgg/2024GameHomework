#include "WeaponBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Course01/Player/CoursePlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

class UEnhancedInputLocalPlayerSubsystem;

AWeaponBase::AWeaponBase()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	WeaponMeshFP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshFP"));
	WeaponMeshFP->SetupAttachment(Root);
	WeaponMeshFP->SetRelativeRotation(FRotator(0, 90, 0));
	WeaponMeshTP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshTP"));
	WeaponMeshTP->SetupAttachment(Root);
	WeaponMeshTP->SetRelativeRotation(FRotator(0, 90, 0));

	bReplicates = true;
}

void AWeaponBase::Shoot(const FVector& FireLocation)
{
	// 生成子弹
	// 目前本地生成后，调用服务器要求其他客户端生成，避免一定的延迟
	// TODO 服务器发射从摄像机的子弹，客户端是枪的子弹，客户端向服务器插值，保证公平性
	SpawnProjectile(FireLocation);
	Server_SpawnProjectile(FireLocation);
}

void AWeaponBase::ReloadWeapon(int32 AddedAmmo)
{
	ActualAmmo = FMath::Clamp(ActualAmmo + AddedAmmo, 0, AmmoPerMag);
	OnRep_ActualAmmo();
	Server_SetActualAmmo(ActualAmmo);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponMeshFP->SetRelativeTransform(FPPWeaponOffsetTransform);
	WeaponMeshTP->SetRelativeTransform(TPPWeaponOffsetTransform);

	ActualAmmo = AmmoPerMag;
	OnRep_ActualAmmo();
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBase, ActualAmmo);
}

void AWeaponBase::SpawnProjectile(const FVector& FireLocation)
{
	if (!IsValid(ProjectileType))
	{
		return;
	}
	AActor* AttachActor = GetOwner();
	ACoursePlayer* Player = Cast<ACoursePlayer>(AttachActor);
	if (!Player)
	{
		return;
	}

	FRotator FacingRotation = Player->GetFacingRotation();
	bool bIsLocalControl = Player->IsLocallyControlled();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	AProjectileBase* Bullet =
		Cast<AProjectileBase>(GetWorld()->SpawnActor(ProjectileType, &FireLocation, &FacingRotation, SpawnParameters));
	Bullet->OnLaunch(FVector(3000.f, 0, 0));
}

void AWeaponBase::Multicast_SpawnProjectile_Implementation(const FVector& FireLocation)
{
	if (ACharacter* Player = Cast<ACharacter>(GetOwner()))
	{
		if (!Player->IsLocallyControlled())
		{
			SpawnProjectile(FireLocation);
		}
	}
}

void AWeaponBase::Server_SpawnProjectile_Implementation(const FVector& FireLocation)
{
	Multicast_SpawnProjectile(FireLocation);
}

void AWeaponBase::Server_SetActualAmmo_Implementation(int32 InActualAmmo)
{
	ActualAmmo = InActualAmmo;
}

void AWeaponBase::OnRep_ActualAmmo()
{
	AmmoChanged.Broadcast();
}
