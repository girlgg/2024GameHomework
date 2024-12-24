#pragma once

#include "CoreMinimal.h"
#include "LuaGameMode.h"
#include "CourseGameMode.generated.h"

UCLASS()
class COURSE01_API ACourseGameMode : public ALuaGameMode
{
	GENERATED_BODY()

public:
	ACourseGameMode();
	//~ GameMode interface
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void SetMatchState(FName NewState) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	//~ End of GameMode interface
protected:
	TArray<APlayerState*> GetPlayersByTeam(int32 InTeamIdx);
	
	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(const FString& TextToSend);
};
