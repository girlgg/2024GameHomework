#include "Course01GameMode.h"
#include "UObject/ConstructorHelpers.h"

ACourse01GameMode::ACourse01GameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void ACourse01GameMode::BeginPlay()
{
	Super::BeginPlay();
}
