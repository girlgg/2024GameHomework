#include "CoursePlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Course01/Components/GameMode/MatchStateSystem.h"
#include "Course01/Components/GameMode/SpawnSystemComponent.h"
#include "Course01/Components/Player/HealthComponent.h"
#include "Course01/Components/Player/InteractSystemComponent.h"
#include "Course01/Components/Player/InventoryComponent.h"
#include "Course01/Components/Player/LoadoutComponent.h"
#include "Course01/Components/Player/PawnInfoComponent.h"
#include "Course01/Components/Player/TeamComponent.h"
#include "Course01/Weapon/WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


ACoursePlayer::ACoursePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0, 0, 70.f));
	SpringArm->TargetArmLength = 0.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(SpringArm);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->SetRelativeLocation(FVector(-15.f, 0.f, -165.f));
	Mesh1P->SetRelativeRotation(FRotator(0.f, -90., 0.f));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->bOwnerNoSee = false;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->CastShadow = true;

	PawnInfoComponent = CreateDefaultSubobject<UPawnInfoComponent>(TEXT("PawnInfoComponent"));
	InventorySystem = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventorySystem"));
	InteractSystem = CreateDefaultSubobject<UInteractSystemComponent>(TEXT("InteractSystem"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	InventorySystem->OnWeaponEquipped.AddDynamic(this, &ThisClass::OnWeaponEquipped);
}

void ACoursePlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (PC->IsLocalPlayerController())
		{
			if (AGameStateBase* GS = UGameplayStatics::GetGameState(GetWorld()))
			{
				if (UMatchStateSystem* MSS = GS->GetComponentByClass<UMatchStateSystem>())
				{
					MSS->OnPostRound.AddDynamic(this, &ThisClass::OnPostRound);
					MSS->OnPostMatch.AddDynamic(this, &ThisClass::OnPostRound);

					if (MSS->GetRoundState() == ERoundState::WaitingToStartRound)
					{
						MSS->OnRoundInProgress.AddDynamic(this, &ThisClass::AddPlayerInputs);
					}
					else
					{
						AddPlayerInputs();
					}
				}
			}
		}
	}
}

void ACoursePlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::EnableDamage, 1.f, false);

	// 创建武器
	ApplySelectedPerk();
}

void ACoursePlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACoursePlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACoursePlayer::Fire(const FInputActionValue& Value)
{
	bWantToFire = true;
	if (CanShoot())
	{
		if (InventorySystem && InventorySystem->CurrentWeapon)
		{
			if (TryToShoot())
			{
				// TODO 播放射击蒙太奇
				FVector EmmitLocation = InventorySystem->CurrentWeapon->WeaponMeshFP->GetSocketLocation(TEXT("muzzle"));
				InventorySystem->CurrentWeapon->Shoot(EmmitLocation);
			}
			else
			{
			}
		}
	}
}

void ACoursePlayer::StopFire(const FInputActionValue& Value)
{
	bWantToFire = false;
}

void ACoursePlayer::EnableDamage()
{
	SetCanBeDamaged(true);
}

bool ACoursePlayer::CanShoot()
{
	return true;
}

bool ACoursePlayer::TryToShoot()
{
	if (InventorySystem && InventorySystem->CurrentWeapon)
	{
		return InventorySystem->CurrentWeapon->GetActualAmmo() > 0;
	}
	return true;
}

void ACoursePlayer::ApplySelectedPerk()
{
	if (!Controller)
	{
		return;
	}
	ULoadoutComponent* Loadout = Controller->GetComponentByClass<ULoadoutComponent>();
	if (!Loadout)
	{
		return;
	}
}

void ACoursePlayer::OnRep_PawnTeam()
{
	AGameStateBase* GS = UGameplayStatics::GetGameState(GetWorld());
	if (!GS)
	{
		return;
	}
	if (USpawnSystemComponent* SSC = GS->GetComponentByClass<USpawnSystemComponent>())
	{
		SSC->OnPlayerSpawned.Broadcast(this);
	}
}

void ACoursePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled() || HasAuthority())
	{
		ControlRotation = GetControlRotation();
	}
}

void ACoursePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACoursePlayer, PawnTeam);
	DOREPLIFETIME_CONDITION(ACoursePlayer, ControlRotation, COND_SimulatedOnly);
}

void ACoursePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ThisClass::Fire);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ThisClass::StopFire);
	}
}

void ACoursePlayer::BecomeViewTarget(APlayerController* PC)
{
	Super::BecomeViewTarget(PC);
	if (HasAuthority())
	{
		SetPawnTeam();
		if (ULoadoutComponent* LoadoutComponent = Controller->GetComponentByClass<ULoadoutComponent>())
		{
			InventorySystem->Server_CreateAndEquipLoadout(LoadoutComponent->GetPrimaryWeaponClass(),
			                                              Mesh1P, GetMesh());
		}
	}
}

void ACoursePlayer::OnPostRound()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		DisableInput(PC);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(InputMappingContext);
		}
	}
	// TODO StopAiming();
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->StopActiveMovement();
	GetCharacterMovement()->DisableMovement();
	// TODO PawnInfo->SetRunning(false);
	bCanShoot = false;
	GetWorld()->GetTimerManager().ClearTimer(AutoShootTimerHandle);
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		PCM->StopAllCameraShakes(false);
	}
}

void ACoursePlayer::AddPlayerInputs()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ACoursePlayer::GetPawnMeshes(USkeletalMeshComponent*& FPP, USkeletalMeshComponent*& TPP)
{
	FPP = Mesh1P;
	TPP = GetMesh();
}

void ACoursePlayer::SetPawnTeam()
{
	if (UTeamComponent* TeamComponent = GetPlayerState()->GetComponentByClass<UTeamComponent>())
	{
		PawnTeam = TeamComponent->GetTeam();
		OnRep_PawnTeam();
	}
}

void ACoursePlayer::OnWeaponEquipped(AWeaponBase* WeaponEquipped)
{
	SetWeaponAnimBlueprints(WeaponEquipped);
}

void ACoursePlayer::SetWeaponAnimBlueprints(const AWeaponBase* WeaponEquipped) const
{
	if (IsValid(WeaponEquipped->FirstPerson_WeaponAnims))
	{
		Mesh1P->LinkAnimClassLayers(WeaponEquipped->FirstPerson_WeaponAnims);
	}
	if (IsValid(WeaponEquipped->ThirdPerson_WeaponAnims))
	{
		GetMesh()->LinkAnimClassLayers(WeaponEquipped->ThirdPerson_WeaponAnims);
	}
}
