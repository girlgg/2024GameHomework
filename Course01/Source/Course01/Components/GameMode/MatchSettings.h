#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MatchSettings.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COURSE01_API UMatchSettings : public UActorComponent
{
	GENERATED_BODY()

public:
	UMatchSettings();

	void SetAllowFriendlyFire(bool bInAllowFriendlyFire);
	void SetPointsToWin(int32 InPointsToWin);
	void SetRoundTimeLimit(float InRoundTimeLimit);
	void SetAllowBots(bool bInAllowBots);

	FORCEINLINE bool GetAllowRespawn() const { return bAllowRespawn; }

protected:
	virtual void BeginPlay() override;

public:
	/* 开始匹配前的等待时间 */
	float PreMatchTime{3.f};
	/* 每回合的时间 */
	float RoundTimeLimit{180.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointsToWin{30};

	uint8 bMatchWithTimeLimit : 1{true};
	uint8 bAllowFriendlyFire : 1{false};
	uint8 bAllowBots : 1{false};

protected:
	uint8 bAllowRespawn : 1{true};
};
