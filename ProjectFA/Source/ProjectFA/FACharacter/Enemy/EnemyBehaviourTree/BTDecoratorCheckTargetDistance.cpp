// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecoratorCheckTargetDistance.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecoratorCheckTargetDistance::UBTDecoratorCheckTargetDistance()
{
	NodeName = TEXT("Check Target Is Near");
}

bool UBTDecoratorCheckTargetDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto StartPosition= OwnerComp.GetBlackboardComponent()->GetValueAsVector(StartPositionKey);
	const auto TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetPlayerKey);
	const auto TargetActor = Cast<AActor>(TargetObject);
	if(TargetActor == nullptr)	return false;
	
	const auto TargetPosition = TargetActor->GetActorLocation();
	const auto Distance = FVector::DistSquared2D(StartPosition, TargetPosition);
	if(Distance > MaxDistanceSqrt)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetPlayerKey, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TargetPlayerIsNearKey, false);
		return false;
	}
	return true;
}