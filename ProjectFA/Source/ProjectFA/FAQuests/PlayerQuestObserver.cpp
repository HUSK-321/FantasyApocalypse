// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerQuestObserver.h"
#include "QuestObject.h"

UPlayerQuestObserver::UPlayerQuestObserver()
{
	
}

void UPlayerQuestObserver::AddEnemyDestroyQuest(UQuestObject* Quest)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("퀘스트 추가")));
	EnemyDestroyQuestList.Add(Quest);
}

void UPlayerQuestObserver::SearchDestroyEnemyQuest(UObject* Enemy)
{
	for(const auto Quest : EnemyDestroyQuestList)
	{
		Quest->CalculateQuest(Enemy);
	}
}