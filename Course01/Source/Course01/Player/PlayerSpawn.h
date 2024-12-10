#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerSpawn.generated.h"

UCLASS()
class COURSE01_API APlayerSpawn : public APlayerStart
{
	GENERATED_BODY()

public:
	APlayerSpawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	bool CheckCanBeUsed() const;
	
	void SetActive(bool bInActive) { bActive = bInActive; }
	FORCEINLINE bool IsActive() const { return bActive; }

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamIdx{0};

	uint8 bActive : 1{true};
};
