// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_WaitForSearchPatrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	auto NextPatrolPoint = UKismetMathLibrary::RandomPointInBoundingBox(OriginPosition, PatrolBoxAreaExtent);

	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(30.f);
	while (World->OverlapAnyTestByChannel(NextPatrolPoint, FQuat(), ECC_Visibility, CollisionShape))
	{
		UE_LOG(LogTemp, Warning, TEXT("search again"));
		NextPatrolPoint = UKismetMathLibrary::RandomPointInBoundingBox(OriginPosition, PatrolBoxAreaExtent);
	}
	
	DrawDebugSphere(World, NextPatrolPoint, 10.f, 12, FColor::Cyan, false, 2.f);
	BlackboardComponent->SetValueAsVector(TEXT("NextPatrolPoint"), NextPatrolPoint);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}