#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreCube.generated.h"

class UBoxComponent;

UCLASS()
class COURSE01_API AScoreCube : public AActor
{
	GENERATED_BODY()

public:
	FORCEINLINE int32 GetScore() const { return bIsImport ? 2 * CubeBaseScore : CubeBaseScore; }

	AScoreCube();

	UFUNCTION(Server, Reliable)
	void Server_OnHit(FVector Impulse);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnHit(FVector Impulse);

	/* 方块每次被击中，获得X分 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 CubeBaseScore{1};
	/* 方块每次被击中，缩放为Y倍 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float CubeScale{0.9f};
	/* 方块被击中多少次后销毁 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 HitTimes{1};
	/* 是否为重要目标，双倍得分 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bIsImport{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Cube;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	UPROPERTY(Replicated)
	int32 CurrentHit{0};
};
