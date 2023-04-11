// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectFAPlayGameMode.generated.h"

/**
 * 
 */

class APlayableCharacter;
class APlayableController;
class APickupItem;

UCLASS()
class PROJECTFA_API AProjectFAPlayGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	void PlayerDead(APlayableCharacter* VictimCharacter, APlayableController* VictimController, APlayableController* InstigatorController);

protected:
	virtual void BeginPlay() override;

private:
	TArray<APickupItem*> GetRandomItemList();

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APickupItem>> ItemTable;
};
