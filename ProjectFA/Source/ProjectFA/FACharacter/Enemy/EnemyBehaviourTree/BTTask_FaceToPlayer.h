// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FaceToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFA_API UBTTask_FaceToPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FaceToPlayer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName TargetPlayerKey;
	
};
