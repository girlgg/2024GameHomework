#pragma once

#include "CoreMinimal.h"
#include "LuaPlayerState.h"
#include "CoursePlayerStateBase.generated.h"

class UTeamComponent;

UCLASS()
class COURSE01_API ACoursePlayerStateBase : public ALuaPlayerState
{
	GENERATED_BODY()
public:
	ACoursePlayerStateBase();
	
	void AddScoreToPlayer(int32 ScoresToAdd);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTeamComponent* TeamComponent;
};
