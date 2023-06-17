// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestObject.h"
#include "EnemyDestroyQuest.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFA_API UEnemyDestroyQuest : public UQuestObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Info", meta = (AllowPrivateAccess = "true"))
	FName TargetEnemyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Info", meta = (AllowPrivateAccess = "true"))
	int32 DestroyCount;

public:
	UEnemyDestroyQuest();

	virtual void CalculateQuest(UObject* TargetObject) override;
	virtual void GetQuestReward() override;

private:
	void DestroyCorrectEnemy();
};
