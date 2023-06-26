// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const int32 NumOfPlayers = GameState.Get()->PlayerArray.Num();
	const auto PlayerName = NewPlayer->GetPlayerState<APlayerState>()->GetPlayerName();
	
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("%s Has Joined Game!!! (Total Players : %d)"), *PlayerName, NumOfPlayers));
	}
	OnNewPlayerJoined.Broadcast(NumOfPlayers);
}