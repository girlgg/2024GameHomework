#pragma once

#include "CoreMinimal.h"
#include "Course01/GameMode/CourseGameMode.h"

#include "ShooterGameMode.generated.h"

class UScoreBoxManager;
class AScoreCube;

UCLASS()
class COURSE01_API AShooterGameMode : public ACourseGameMode
{
	GENERATED_BODY()
public:
	AShooterGameMode();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UScoreBoxManager* ScoreBoxManager;
};
