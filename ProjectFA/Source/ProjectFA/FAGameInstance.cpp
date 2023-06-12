// Fill out your copyright notice in the Description page of Project Settings.


#include "FAGameInstance.h"
#include "FADictionary/FACoreDelegates.h"
#include "FAQuests/PlayerQuestManagement.h"

UFAGameInstance::UFAGameInstance()
{
}

void UFAGameInstance::Init()
{
	Super::Init();
	
	FACoreDelegates::OnEnemyDestroyed.AddUFunction(this, FName("SearchDestroyEnemyQuest"));
	
	if(PlayerQuestManagementClass)
	{
		PlayerQuestManagement = NewObject<UPlayerQuestManagement>(this, PlayerQuestManagementClass);
		if(PlayerQuestManagement && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("PlayerQuestManagement 만듬")));
		}
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
