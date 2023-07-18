// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckTargetIsNear.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectFA/FACharacter/FACharacter.h"

UBTService_CheckTargetIsNear::UBTService_CheckTargetIsNear()
{
	NodeName = TEXT("Check Target Is Near");
	Interval = 1.0f;
	RandomDeviation = 0.f;
}

void UBTService_CheckTargetIsNear::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if(OwnerComp.GetBlackboardComponent() == nullptr)	return;
	const auto AIController = OwnerComp.GetAIOwner();
	if(AIController == nullptr)	return;
	const auto Pawn = AIController->GetPawn();
	if(Pawn == nullptr)	return;
	const UObject* ObjectToSearch = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey);
	if(ObjectToSearch == nullptr)	return;

	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) };
	const TArray<AActor*> ActorToIgnore { Pawn };
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(Pawn, Pawn->GetActorLocation(), SearchRadius, ObjectTypes, AFACharacter::StaticClass(), ActorToIgnore, OutActors);

#if WITH_EDITOR
	UKismetSystemLibrary::DrawDebugSphere(Pawn, Pawn->GetActorLocation(), SearchRadius, 30, FLinearColor::Green, .5f, 1.f);
#endif

	for(const auto OverlappedActor : OutActors)
	{
		if(ObjectToSearch != OverlappedActor)	continue;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TargetIsNearKey, true);
		return;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TargetIsNearKey, false);
}