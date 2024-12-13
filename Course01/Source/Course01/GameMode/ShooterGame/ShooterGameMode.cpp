#include "ShooterGameMode.h"

#include "Course01/Components/GameMode/ScoreBoxManager.h"

AShooterGameMode::AShooterGameMode()
{
	ScoreBoxManager = CreateDefaultSubobject<UScoreBoxManager>(TEXT("ScoreBoxManager"));
}
