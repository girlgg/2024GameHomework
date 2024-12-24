#include "MenuGameMode.h"

#include "MenuGameState.h"

AMenuGameMode::AMenuGameMode()
{
	GameStateClass = AMenuGameState::StaticClass();
}

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
}