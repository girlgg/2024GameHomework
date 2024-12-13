#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "ScoreBoxManager.generated.h"


class AScoreCube;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COURSE01_API UScoreBoxManager : public USceneComponent
{
	GENERATED_BODY()

public:
	UScoreBoxManager();

	void AddScoreCube(AScoreCube* ScoreCube);
	void AddCubeSpawnLocation(FVector InLocation);

	void SpawnRandomCube(bool bIsImport);

protected:
	virtual void BeginPlay() override;

	/* 初始随机生成多少个方块 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BoxNum{1};
	/* 开局 N 个方块变为重要目标 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ImportNum{1};
	/* 方块每次被击中，获得X分 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 CubeBaseScore{1};
	/* 方块每次被击中，缩放为Y倍 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float CubeScale{0.9f};
	/* 方块被击中多少次后销毁 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 HitTimes{1};

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FVector> SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AScoreCube> ScoreCubeClass;
	
	TArray<AScoreCube*> ScoreCubes;
};
