// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNewPlayerJoined, int32);

UCLASS()
class PROJECTFA_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	FOnNewPlayerJoined OnNewPlayerJoined;

public:
	ALobbyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};