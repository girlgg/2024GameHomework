#include "CourseGameStateBase.h"

#include "Blueprint/UserWidget.h"
#include "Course01/CourseGameInstance.h"
#include "Course01/Components/GameMode/MatchSettings.h"
#include "Course01/Components/GameMode/MatchStateSystem.h"
#include "Course01/Components/GameMode/SpawnSystemComponent.h"
#include "Course01/Components/Player/TeamComponent.h"
#include "Course01/GameObjectives/ObjectiveBase.h"
#include "Course01/UserInterface/GamePlay/EndRoundWidget.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACourseGameStateBase::ACourseGameStateBase()
{
	MatchSettings = CreateDefaultSubobject<UMatchSettings>(TEXT("MatchSettings"));
	MatchStateSystem = CreateDefaultSubobject<UMatchStateSystem>(TEXT("MatchStateSystem"));
	SpawnSystem = CreateDefaultSubobject<USpawnSystemComponent>(TEXT("SpawnSystem"));
}

void ACourseGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (MatchStateSystem)
	{
		MatchStateSystem->OnPostRound.AddDynamic(this, &ThisClass::OnPostRound);
	}
	
	if (!HasAuthority() || MatchStateSystem->GetIsMenu())
	{
		return;
	}
	SetMatchSettings();
	CreateBots();
	GetObjectives();
}

void ACourseGameStateBase::Reset()
{
	Super::Reset();
	if (MatchStateSystem->GetIsMenu())
	{
		return;
	}
	MatchStateSystem->OnReset();
	FlipTeams();
	MatchStateSystem->FlipRoundsWon();
	GetObjectives();
}

void ACourseGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACourseGameStateBase, Objectives);
	DOREPLIFETIME(ACourseGameStateBase, MatchStateSystem);
}

void ACourseGameStateBase::GetGameModeWidgets(TSoftClassPtr<UUserWidget>& OutGameModeWidget,
                                              TSoftClassPtr<UUserWidget>& OutScoresBoard,
                                              TSoftClassPtr<UUserWidget>& OutEndGmeScores)
{
	OutGameModeWidget = GameModeWidgetClass;
	OutScoresBoard = ScoresBoardClass;
	OutEndGmeScores = EndGmeScoresClass;
}

void ACourseGameStateBase::SpawnPlayer(APlayerController* InPlayerController)
{
	if (MatchStateSystem->IsGameOver() || MatchStateSystem->IsRoundOver())
	{
		return;
	}
	SpawnSystem->Server_SpawnPlayerWithTeam(InPlayerController);
}

void ACourseGameStateBase::SetMatchSettings()
{
	if (UCourseGameInstance* CGI = Cast<UCourseGameInstance>(GetGameInstance()))
	{
		if (CGI->bUseCustomSettings)
		{
			MatchSettings->SetAllowFriendlyFire(CGI->bAllowFriendlyFire);
			MatchSettings->SetPointsToWin(CGI->PointsToWin);
			MatchSettings->SetRoundTimeLimit(CGI->RoundTimeLimit);
			MatchSettings->SetAllowBots(CGI->bAllowBots);
			if (CGI->bAllowBots)
			{
			}
		}
	}
}

void ACourseGameStateBase::CreateBots()
{
}

void ACourseGameStateBase::GetObjectives()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjectiveBase::StaticClass(), Objectives);
}

void ACourseGameStateBase::FlipTeams()
{
	for (APlayerState* PS : PlayerArray)
	{
		if (UTeamComponent* Team = PS->GetComponentByClass<UTeamComponent>())
		{
			int32 TeamId = Team->GetTeam() == 0;
			Team->Server_SetTeam(TeamId);
		}
	}
}

void ACourseGameStateBase::OnPostRound()
{
	if(UKismetSystemLibrary::IsDedicatedServer(GetWorld()))
	{
		return;
	}
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		TSubclassOf<UEndRoundWidget> EndRoundClass = EndRoundScoresClass.LoadSynchronous();
		if (EndRoundClass)
		{
			if (UEndRoundWidget* EndRoundWidget = CreateWidget<UEndRoundWidget>(PlayerController, EndRoundClass))
			{
				EndRoundWidget->AddToViewport();
			}
		}
	}
}
