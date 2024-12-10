#pragma once

#include "CoreMinimal.h"
#include "slua.h"
#include "Engine/GameInstance.h"
#include "CourseGameInstance.generated.h"

UCLASS()
class COURSE01_API UCourseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCourseGameInstance();
	//~ GameInstance interface
	virtual void Shutdown() override;
	//~ end of GameInstance interface

	void CloseLuaState();
	void CreateLuaState();

	void LuaStateInitCallback(NS_SLUA::lua_State* L);

	int32 PointsToWin{200};

	float RoundTimeLimit{120.f};

	bool bUseCustomSettings{false};
	bool bAllowFriendlyFire{false};
	bool bAllowBots{false};

	NS_SLUA::LuaState* state{nullptr};
};
