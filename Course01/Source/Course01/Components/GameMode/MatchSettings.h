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
	FORCEINLINE int32 GetRoundsToWin() const {return RoundsToWin;}
	FORCEINLINE float GetPostRoundTime() const {return PostRoundTime;}

protected:
	virtual void BeginPlay() override;

public:
	/* 开始匹配前的等待时间 */
	float PreMatchTime{3.f};
	/* 每回合的时间 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RoundTimeLimit{60.f};

	float PostRoundTime{5.f};
	/* 某队达到多少分赢得本局胜利 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointsToWin{30};
	/* 某队达到多少局赢得本次匹配胜利 */
	int32 RoundsToWin{1};

	uint8 bMatchWithTimeLimit : 1{true};
	uint8 bAllowFriendlyFire : 1{false};
	uint8 bAllowBots : 1{false};

protected:
	uint8 bAllowRespawn : 1{true};
};
