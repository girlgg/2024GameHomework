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
	AScoreCube();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;
};
