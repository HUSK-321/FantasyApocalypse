// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_WaitForSearchPatrol.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFA_API UBTTask_WaitForSearchPatrol : public UBTTask_Wait
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
	FVector PatrolBoxAreaExtent;

public:
	UBTTask_WaitForSearchPatrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};