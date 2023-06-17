// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDestroyQuest.h"
#include "ProjectFA/FACharacter/FACharacter.h"

UEnemyDestroyQuest::UEnemyDestroyQuest()
{
	QuestID = 1;
	QuestTitle = TEXT("테스트로 적 죽여오기");
	QuestDescription = TEXT("공룡 1마리 때려죽이기");
	DestroyCount = 1;
}

void UEnemyDestroyQuest::CalculateQuest(UObject* TargetObject)
{
	Super::CalculateQuest(TargetObject);

	if(IsQuestCompleted())	return;

	const auto Enemy = Cast<AFACharacter>(TargetObject);
	if(Enemy == nullptr)	return;
	if(TargetEnemyName == Enemy->GetCharacterName())
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Enemy 이름이 같다")));
		DestroyCorrectEnemy();
	}
}

void UEnemyDestroyQuest::GetQuestReward()
{
	Super::GetQuestReward();
}

void UEnemyDestroyQuest::DestroyCorrectEnemy()
{
	DestroyCount -= 1;
	if(DestroyCount <= 0)
	{
		SetQuestComplete();
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Enemy 전부 처치 완료 따이")));
	}
}
