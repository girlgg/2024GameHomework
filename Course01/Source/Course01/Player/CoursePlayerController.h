#pragma once

#include "CoreMinimal.h"
#include "LuaPlayerController.h"
#include "CoursePlayerController.generated.h"

class ULoadoutComponent;
class UWaitingToStartWidget;
class UUIManagerComponent;
class UInputMappingContext;

UCLASS()
class COURSE01_API ACoursePlayerController : public ALuaPlayerController
{
	GENERATED_BODY()

public:
	ACoursePlayerController();

	void Spawn();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void PossessedPawnChanged(APawn* InOldPawn, APawn* InNewPawn);

	UFUNCTION(Server, Reliable)
	void Server_SpawnPlayer();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<UWaitingToStartWidget> WaitingToStartWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UUIManagerComponent* UIManagerComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULoadoutComponent* LoadoutComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInputMappingContext* ControllerInputs;
};
