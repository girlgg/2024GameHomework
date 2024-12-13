#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CourseGameStateBase.generated.h"

class UEndRoundWidget;
class USpawnSystemComponent;
class UMatchStateSystem;
class UMatchSettings;

UCLASS()
class COURSE01_API ACourseGameStateBase : public AGameState
{
	GENERATED_BODY()

public:
	ACourseGameStateBase();

	//~ Actor interface
	virtual void BeginPlay() override;
	virtual void Reset() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End of Actor interface

	void GetGameModeWidgets(TSoftClassPtr<UUserWidget>& OutGameModeWidget,
	                        TSoftClassPtr<UUserWidget>& OutScoresBoard,
	                        TSoftClassPtr<UUserWidget>& OutEndGmeScores);

	void SpawnPlayer(APlayerController* InPlayerController);

protected:
	void SetMatchSettings();
	void CreateBots();
	void GetObjectives();
	void FlipTeams();

	UFUNCTION()
	void OnPostRound();

	UPROPERTY(Replicated)
	TArray<AActor*> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMatchSettings* MatchSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UMatchStateSystem* MatchStateSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpawnSystemComponent* SpawnSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> GameModeWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UEndRoundWidget> EndRoundScoresClass;

	TSoftClassPtr<UUserWidget> ScoresBoardClass;
	TSoftClassPtr<UUserWidget> EndGmeScoresClass;
};
