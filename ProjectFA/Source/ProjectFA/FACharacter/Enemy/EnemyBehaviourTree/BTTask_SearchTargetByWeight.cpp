// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SearchTargetByWeight.h"
#include "AIController.h"
#include "ProjectFA/FAInterfaces/Controller/EnemyControllable.h"

UBTTask_SearchTargetByWeight::UBTTask_SearchTargetByWeight()
{
	NodeName = TEXT("Search Target By Weight");
}

EBTNodeResult::Type UBTTask_SearchTargetByWeight::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	const auto EnemyController = ControllingPawn->GetController<IEnemyControllable>();
	if(EnemyController == nullptr)	return EBTNodeResult::Failed;

	EnemyController->SetEnemyBlackboardValueAsObject(ObjectKeyToSearch, nullptr, 0.f);
	return EBTNodeResult::Succeeded;
}