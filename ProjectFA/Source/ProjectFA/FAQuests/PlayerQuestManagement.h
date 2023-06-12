// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerQuestManagement.generated.h"

class UQuestObject;

UCLASS(Blueprintable, BlueprintType)
class PROJECTFA_API UPlayerQuestManagement : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UQuestObject>> EnemyDestroyQuestList;
	
public:
	UPlayerQuestManagement();
	
	UFUNCTION(BlueprintCallable)
	void AddEnemyDestroyQuest(UQuestObject* Quest);
	UFUNCTION(BlueprintCallable)
	void SearchDestroyEnemyQuest(UObject* Enemy);
	
private:
	
};