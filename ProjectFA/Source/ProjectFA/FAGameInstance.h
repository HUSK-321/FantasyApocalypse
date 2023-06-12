// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FAInterfaces/QuestObservable.h"
#include "FAGameInstance.generated.h"

class UPlayerQuestObserver;

UCLASS()
class PROJECTFA_API UFAGameInstance : public UGameInstance, public IQuestObservable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Quest")
	TSubclassOf<UPlayerQuestObserver> PlayerQuestManagementClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Quest")
	TObjectPtr<UPlayerQuestObserver> PlayerQuestManagement;

public:
	UFAGameInstance();
	virtual void Init() override;

	virtual void AddEnemyDestroyQuest_Implementation(UQuestObject* Quest) override;
	UFUNCTION()
	void SearchDestroyEnemyQuest(UObject* Enemy);
};