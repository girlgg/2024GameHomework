#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameUIPolicy.generated.h"

class UCourseLocalPlayer;
class UPrimaryGameLayout;

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPrimaryGameLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	FRootViewportLayoutInfo()
	{
	}

	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		  , RootLayout(InRootLayout)
		  , bAddedToViewport(bIsInViewport)
	{
	}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }
};

UCLASS(Blueprintable, Within = GameUIManagerSubsystem)
class COURSE01_API UGameUIPolicy : public UObject
{
	GENERATED_BODY()

public:
	UPrimaryGameLayout* GetRootLayout(const UCourseLocalPlayer* LocalPlayer) const;

protected:
	void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	void CreateLayoutWidget(ULocalPlayer* LocalPlayer);

private:
	friend class UGameUIManagerSubsystem;

	void NotifyPlayerAdded(UCourseLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UCourseLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UCourseLocalPlayer* LocalPlayer);

	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> LayoutClass;
};
