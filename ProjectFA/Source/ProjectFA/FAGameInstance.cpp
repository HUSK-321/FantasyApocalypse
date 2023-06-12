// Fill out your copyright notice in the Description page of Project Settings.


#include "FAGameInstance.h"
#include "FADictionary/FACoreDelegates.h"
#include "FAQuests/PlayerQuestObserver.h"

UFAGameInstance::UFAGameInstance()
{
}

void UFAGameInstance::Init()
{
	Super::Init();
	
	FACoreDelegates::OnEnemyDestroyed.AddUFunction(this, FName("SearchDestroyEnemyQuest"));
	
	if(PlayerQuestManagementClass)
	{
		PlayerQuestManagement = NewObject<UPlayerQuestObserver>(this, PlayerQuestManagementClass);
	}
}

void UFAGameInstance::AddEnemyDestroyQuest_Implementation(UQuestObject* Quest)
{
	PlayerQuestManagement->AddEnemyDestroyQuest(Quest);
}

void UFAGameInstance::SearchDestroyEnemyQuest(UObject* Enemy)
{
	PlayerQuestManagement->SearchDestroyEnemyQuest(Enemy);
}
