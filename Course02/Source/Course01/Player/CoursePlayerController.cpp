#include "CoursePlayerController.h"

#include "CourseLocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Course01/Components/GameMode/MatchStateSystem.h"
#include "Course01/Components/GameMode/UIManagerComponent.h"
#include "Course01/Components/Player/LoadoutComponent.h"
#include "Course01/GameMode/CourseGameStateBase.h"
#include "Course01/UserInterface/GamePlay/WaitingToStartWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ACoursePlayerController::ACoursePlayerController()
{
	UIManagerComponent = CreateDefaultSubobject<UUIManagerComponent>(TEXT("UIManagerComponent"));
	LoadoutComponent = CreateDefaultSubobject<ULoadoutComponent>(TEXT("LoadoutComponent"));
}

void ACoursePlayerController::Spawn()
{
	Server_SpawnPlayer();
}

void ACoursePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalPlayerController())
	{
		return;
	}
	OnPossessedPawnChanged.AddDynamic(this, &ThisClass::PossessedPawnChanged);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (ControllerInputs)
		{
			Subsystem->AddMappingContext(ControllerInputs, 0);
		}
	}
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	SetViewTargetWithBlend(Actors[0]);
}

void ACoursePlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (UCourseLocalPlayer* LocalPlayer = Cast<UCourseLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);
	}
}

void ACoursePlayerController::PossessedPawnChanged(APawn* InOldPawn, APawn* InNewPawn)
{
	if (Cast<ACharacter>(InNewPawn))
	{
		if (AGameStateBase* GS = UGameplayStatics::GetGameState(GetWorld()))
		{
			if (UMatchStateSystem* MSS = GS->GetComponentByClass<UMatchStateSystem>())
			{
				if (MSS->GetRoundState() == ERoundState::WaitingToStartRound)
				{
					if (UClass* LoadedClass = WaitingToStartWidget.LoadSynchronous())
					{
						UWaitingToStartWidget* Widget =
							CreateWidget<UWaitingToStartWidget>(this, LoadedClass);
						Widget->AddToViewport();
					}
				}
			}
		}
	}
}

void ACoursePlayerController::Server_SpawnPlayer_Implementation()
{
	UnPossess();
	if (ACourseGameStateBase* CGSB = Cast<ACourseGameStateBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		CGSB->SpawnPlayer(this);
	}
}
