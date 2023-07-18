// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_WaitForSearchPatrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_WaitForSearchPatrol::UBTTask_WaitForSearchPatrol()
{
	NodeName = TEXT("Wait For Search Patrol Point");
}

EBTNodeResult::Type UBTTask_WaitForSearchPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	const auto World = OwnerComp.GetWorld();
	if(BlackboardComponent == nullptr || World == nullptr)	return EBTNodeResult::Failed;

	const auto OriginPosition = BlackboardComponent->GetValueAsVector(TEXT("PatrolStartPoint"));
	const auto NavSystem = UNavigationSystemV1::GetNavigationSystem(World);
	if(NavSystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTTask_WaitForSearchPatrol] : In this world Navigation system is null"))
		return EBTNodeResult::Failed;
	}
	
	FNavLocation NextNavLocation;
	if(NavSystem->GetRandomPointInNavigableRadius(OriginPosition, PatrolRadius, NextNavLocation))
	{
		
#if WITH_EDITOR
		DrawDebugSphere(World, NextNavLocation.Location, 10.f, 12, FColor::Cyan, false, 2.f);
#endif
		
		BlackboardComponent->SetValueAsVector(TEXT("NextPatrolPoint"), NextNavLocation.Location);
		return Super::ExecuteTask(OwnerComp, NodeMemory);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[UBTTask_WaitForSearchPatrol] : GetRandomPointInNavigableRadius Fail"))
	return EBTNodeResult::Failed;
}