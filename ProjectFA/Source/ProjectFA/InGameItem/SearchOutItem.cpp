// Fill out your copyright notice in the Description page of Project Settings.


#include "SearchOutItem.h"

#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectFA/FAInterfaces/SearchOutEffectable.h"
#include "ProjectFA/FAInterfaces/Controller/ItemRPCableController.h"

ASearchOutItem::ASearchOutItem()
{
	
}

void ASearchOutItem::InventoryAction_Implementation()
{
	UseAction();
}

void ASearchOutItem::RemoveFromInventoryAction_Implementation()
{
	const auto OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr)	return;
	if(const auto OwnerController = OwnerPawn->GetController<IItemRPCableController>())
	{
		OwnerController->DropItem(this);
	}
}

void ASearchOutItem::UseAction()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), 1000.f, ObjectTypeQuery,
		ACharacter::StaticClass(), ActorsToIgnore, OutActors);

	for(auto SearchedActor : OutActors)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(SearchedActor, USearchOutEffectable::StaticClass()))
		{
			SearchOutList.Emplace(SearchedActor);
		}
	}

	for(auto SearchOutACtor : SearchOutList)
	{
		if(auto SearchOutEffectableActor = Cast<ISearchOutEffectable>(SearchOutACtor))
		{
			SearchOutEffectableActor->EnableSearchOutEffect();
		}
	}

	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(SearchOutTimer, this, &ASearchOutItem::ResetSearchOutActors, SearchOutTime);
	}
}

void ASearchOutItem::ResetSearchOutActors()
{
	for(auto SearchOutACtor : SearchOutList)
	{
		if(auto SearchOutEffectableActor = Cast<ISearchOutEffectable>(SearchOutACtor))
		{
			SearchOutEffectableActor->DisableSearchOutEffect();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[APlayableCharacter::DisableSearchOutEffect]"));

	if(GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}

	Destroy();
}
