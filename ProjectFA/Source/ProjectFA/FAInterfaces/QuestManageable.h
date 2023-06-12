// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestManageable.generated.h"

UINTERFACE(MinimalAPI)
class UQuestManageable : public UInterface
{
	GENERATED_BODY()
};


class UQuestObject;

class PROJECTFA_API IQuestManageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Quest Manage")
	void AddEnemyDestroyQuest(UQuestObject* Quest);
};
