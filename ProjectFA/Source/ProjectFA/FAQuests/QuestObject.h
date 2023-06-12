// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestObject.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PROJECTFA_API UQuestObject : public UObject
{
	GENERATED_BODY()

public:
	UQuestObject();

	UFUNCTION(BlueprintCallable)
	virtual void CalculateQuest(UObject* TargetObject) {}
	UFUNCTION(BlueprintCallable)
	virtual void GetQuestReward() {}

protected:
	void SetQuestComplete();
	bool IsQuestCompleted() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Info", meta = (AllowPrivateAccess = "true"))
	int32 QuestID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Info", meta = (AllowPrivateAccess = "true"))
	FString QuestTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Info", meta = (AllowPrivateAccess = "true"))
	FString QuestDescription;

private:
	int8 bQuestCompleted : 1;
};