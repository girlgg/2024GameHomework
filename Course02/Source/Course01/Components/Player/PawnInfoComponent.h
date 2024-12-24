#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnInfoComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COURSE01_API UPawnInfoComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPawnInfoComponent();

protected:
	virtual void BeginPlay() override;
};
