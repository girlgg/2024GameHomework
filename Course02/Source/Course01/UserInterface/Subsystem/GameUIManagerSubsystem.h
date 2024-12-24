#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIManagerSubsystem.generated.h"

class UCourseLocalPlayer;
class UGameUIPolicy;

UCLASS(Config = Game)
class COURSE01_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGameUIManagerSubsystem();

	//~ Subsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End of Subsystem interface

	const UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }
	
	virtual void NotifyPlayerAdded(UCourseLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UCourseLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UCourseLocalPlayer* LocalPlayer);
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy{nullptr};
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
};
