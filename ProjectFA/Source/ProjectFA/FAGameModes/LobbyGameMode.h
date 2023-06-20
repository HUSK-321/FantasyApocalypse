// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"


UCLASS()
class PROJECTFA_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby GameMode", meta = (AllowPrivateAccess = "true"))
	int32 PlayerNumberToStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby GameMode", meta = (AllowPrivateAccess = "true"))
	FString LevelPathToPlayGame;

	FTimerHandle StartTimerHandle;

public:
	ALobbyGameMode();
	void TravelToLevel();
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	void StartGameTimer(); 
	bool NowStartGameTimerOn() const;
	
};
