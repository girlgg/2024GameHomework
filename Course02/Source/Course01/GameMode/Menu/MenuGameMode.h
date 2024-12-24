#pragma once

#include "CoreMinimal.h"
#include "Course01/GameMode/CourseGameMode.h"
#include "MenuGameMode.generated.h"

UCLASS()
class COURSE01_API AMenuGameMode : public ACourseGameMode
{
	GENERATED_BODY()

public:
	AMenuGameMode();

protected:
	virtual void BeginPlay() override;
};
