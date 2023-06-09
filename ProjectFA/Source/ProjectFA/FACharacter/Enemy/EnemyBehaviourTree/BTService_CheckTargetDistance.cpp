// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckTargetDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectFA/FAInterfaces/Controller/EnemyControllable.h"

UBTService_CheckTargetDistance::UBTService_CheckTargetDistance()
{
	NodeName = TEXT("Check Target Distance");
	Interval = 1.0f;
	RandomDeviation = 0.f;
}

void UBTService_CheckTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto StartPosition= OwnerComp.GetBlackboardComponent()->GetValueAsVector(StartPositionKey);
	const auto TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetPlayerKey);
	const auto TargetActor = Cast<AActor>(TargetObject);
	if(TargetActor == nullptr)	return;
	
	const auto TargetPosition = TargetActor->GetActorLocation();
	const auto Distance = FVector::DistSquared2D(StartPosition, TargetPosition);
	if(Distance < MaxDistanceSqrt)	return;
	
	const auto EnemyController = OwnerComp.GetOwner<IEnemyControllable>();
	if(EnemyController == nullptr)	return;

	EnemyController->SetEnemyBlackboardValueAsObject(TargetPlayerKey, TargetActor, 0.f);
	EnemyController->SetEnemyBlackboardValueAsBool(TargetPlayerIsNearKey, false);
}