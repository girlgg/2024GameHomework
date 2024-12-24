#include "InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "Course01/Player/CoursePlayer.h"
#include "Course01/UserInterface/Weapon/WeaponCrosshairWidget.h"
#include "Course01/Weapon/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, PrimaryWeapon);
	DOREPLIFETIME(UInventoryComponent, CurrentWeapon);
}

void UInventoryComponent::Server_CreateAndEquipLoadout_Implementation(TSubclassOf<AWeaponBase> WeaponToSpawn,
                                                                      USkeletalMeshComponent* FPPMesh,
                                                                      USkeletalMeshComponent* TPPMesh)
{
	CreateAndEquipLoadout(WeaponToSpawn, FPPMesh, TPPMesh);
}

void UInventoryComponent::CreateAndEquipLoadout(const TSubclassOf<AWeaponBase>& PrimaryWeaponToSpawn,
                                                USkeletalMeshComponent* FPPMesh,
                                                USkeletalMeshComponent* TPPMesh)
{
	if (!IsValid(PrimaryWeaponToSpawn))
	{
		return;
	}
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}
	APawn* OwnerPawn = Cast<APawn>(OwnerActor);
	if (!OwnerPawn)
	{
		return;
	}
	FTransform SpawnTransform = OwnerPawn->GetActorTransform();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = OwnerPawn;
	SpawnParameters.Instigator = OwnerPawn;

	AWeaponBase* NewWeapon = Cast<AWeaponBase>(
		GetWorld()->SpawnActor(PrimaryWeaponToSpawn, &SpawnTransform, SpawnParameters));
	PrimaryWeapon = NewWeapon;
	OnRep_PrimaryWeapon();
	PrimaryWeapon->SetOwner(GetOwner());
	AttachWeapon(PrimaryWeapon, FPPMesh, TPPMesh);

	CurrentWeapon = PrimaryWeapon;
	OnRep_CurrentWeapon();
	if (CurrentWeapon)
	{
		CurrentWeapon->WeaponMeshFP->SetVisibility(true);
	}
}

void UInventoryComponent::AttachWeapon(AWeaponBase* WeaponToSpawn, USkeletalMeshComponent* FPPMesh,
                                       USkeletalMeshComponent* TPPMesh)
{
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative,
	                                                   EAttachmentRule::KeepRelative, true);
	if (!WeaponToSpawn->WeaponMeshFP->AttachToComponent(FPPMesh, AttachmentTransformRules,TEXT("hand_r")))
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("[UInventoryComponent]: The weapon FPPMesh cannot be attached to the FirstPerson hand_r"));
	}
	if (!WeaponToSpawn->WeaponMeshTP->AttachToComponent(TPPMesh, AttachmentTransformRules,TEXT("hand_r")))
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("[UInventoryComponent]: The weapon TPPMesh cannot be attached to the ThirdPerson hand_r"));
	}
}

void UInventoryComponent::ShowWeaponCrosshair()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick([&]()
	{
		if (CurrentCrosshairWidget)
		{
			CurrentCrosshairWidget->RemoveFromParent();
			CurrentCrosshairWidget = nullptr;
		}
		if (UClass* LoadedClass = CurrentWeapon->CrosshairWidget.LoadSynchronous())
		{
			CurrentCrosshairWidget =
				CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(), 0), LoadedClass);
		}
		if (CurrentCrosshairWidget)
		{
			CurrentCrosshairWidget->AddToViewport();
		}
	});
}

void UInventoryComponent::OnRep_PrimaryWeapon()
{
	ACoursePlayer* Player = Cast<ACoursePlayer>(GetOwner());
	if (!Player)
	{
		return;
	}
	USkeletalMeshComponent *FPP, *TPP;
	Player->GetPawnMeshes(FPP, TPP);
	AttachWeapon(PrimaryWeapon, FPP, TPP);
	PrimaryWeapon->WeaponMeshFP->SetVisibility(CurrentWeapon == PrimaryWeapon);
	PrimaryWeapon->WeaponMeshTP->SetVisibility(CurrentWeapon == PrimaryWeapon);
}

void UInventoryComponent::OnRep_CurrentWeapon()
{
	OnWeaponEquipped.Broadcast(CurrentWeapon);
	APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!LocalPlayerPawn)
	{
		return;
	}
	if (GetOwner() == LocalPlayerPawn && LocalPlayerPawn->IsLocallyControlled())
	{
		ShowWeaponCrosshair();
	}
	else
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->WeaponMeshTP->SetVisibility(true);
		}
		if (PreviousWeapon)
		{
			PreviousWeapon->WeaponMeshTP->SetVisibility(false);
		}
	}
	PreviousWeapon = CurrentWeapon;
}
