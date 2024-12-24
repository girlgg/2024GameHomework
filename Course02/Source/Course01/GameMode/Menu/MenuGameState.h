#pragma once

#include "CoreMinimal.h"
#include "ControlFlowNode.h"
#include "LoadingProcessInterface.h"
#include "Course01/GameMode/CourseGameStateBase.h"
#include "MenuGameState.generated.h"

class UCommonActivatableWidget;

UCLASS()
class COURSE01_API AMenuGameState : public AGameState, public ILoadingProcessInterface
{
	GENERATED_BODY()

protected:
	//~ GameState interface
	virtual void BeginPlay() override;
	//~ End of GameState interface

	//~ ILoadingProcessInterface interface
	virtual bool ShouldShowLoadingScreen() const override;
	//~ End of ILoadingProcessInterface interface

private:
	void FlowStep_TryJoinRequestedSession(FControlFlowNodeRef SubFlow);
	void FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow);

	TSharedPtr<FControlFlow> FrontEndFlow;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> MenuClass;

private:
	bool bShouldShowLoadingScreen = true;
};
