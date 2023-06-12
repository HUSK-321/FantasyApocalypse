// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FAGameInstance.generated.h"

class UPlayerQuestManagement;

UCLASS()
class PROJECTFA_API UFAGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Quest")
	TSubclassOf<UPlayerQuestManagement> PlayerQuestManagementClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Quest")
	TObjectPtr<UPlayerQuestManagement> PlayerQuestManagement;

public:
	UFAGameInstance();
	virtual void Init() override;
};