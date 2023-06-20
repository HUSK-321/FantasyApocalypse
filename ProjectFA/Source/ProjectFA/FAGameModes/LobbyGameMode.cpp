// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

ALobbyGameMode::ALobbyGameMode()
	:
	PlayerNumberToStart(2)
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const int32 NumOfPlayers = GameState.Get()->PlayerArray.Num();
	if(NumOfPlayers == PlayerNumberToStart)
	{
		StartGameTimer();
	}
}

void ALobbyGameMode::StartGameTimer()
{
	if(NowStartGameTimerOn() || GetWorld() == nullptr)	return;

	GetWorld()->GetTimerManager().SetTimer(StartTimerHandle, this, &ALobbyGameMode::TravelToLevel, 10.f, false, 10.f);
}

bool ALobbyGameMode::NowStartGameTimerOn() const
{
	if(GetWorld() == nullptr)	return false;

	return GetWorld()->GetTimerManager().IsTimerActive(StartTimerHandle);
}

void ALobbyGameMode::TravelToLevel()
{
	if(UWorld* World = GetWorld())
	{
		const FString LevelToTravel = FString::Printf(TEXT("%s?listen"), *LevelPathToPlayGame);
		World->ServerTravel(LevelToTravel);
	}
}
