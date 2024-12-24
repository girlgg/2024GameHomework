#include "CoursePlayerStateBase.h"

#include "Course01/Components/Player/TeamComponent.h"

ACoursePlayerStateBase::ACoursePlayerStateBase()
{
	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("TeamComponent"));
}

void ACoursePlayerStateBase::AddScoreToPlayer(int32 ScoresToAdd)
{
}
