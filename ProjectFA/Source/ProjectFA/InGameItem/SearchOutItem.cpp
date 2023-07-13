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
	SearchOutByOverlap();
	EnableSearchOutEffect();

	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(SearchOutTimer, this, &ASearchOutItem::ResetSearchOutActors, SearchOutTime);
	}
	// Remove Item From Inventory, without destroy
	ItemDroppedEvent.Broadcast(this);
}

void ASearchOutItem::ResetSearchOutActors()
{
	DisableSearchOutEffect();

	if(GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}

	Destroy();
}

void ASearchOutItem::SearchOutByOverlap()
{
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;
	const TArray<AActor*> ActorsToIgnore { GetOwner() };
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
}

void ASearchOutItem::EnableSearchOutEffect()
{
	int32 SearchOutCount = 0;
	for(const auto SearchOutActor : SearchOutList)
	{
		if(auto SearchOutEffectableActor = Cast<ISearchOutEffectable>(SearchOutActor))
		{
			SearchOutEffectableActor->EnableSearchOutEffect();
			++SearchOutCount;
		}
	}

	if(const auto OwnerPlayer = Cast<ISearchOutEffectable>(GetOwner()))
	{
		OwnerPlayer->AnnounceToOwner(SearchOutCount);
		ActorToAnnounceEnd = GetOwner();
	}
}

void ASearchOutItem::DisableSearchOutEffect()
{
	for(auto SearchOutActor : SearchOutList)
	{
		if(auto SearchOutEffectableActor = Cast<ISearchOutEffectable>(SearchOutActor))
		{
			SearchOutEffectableActor->DisableSearchOutEffect();
		}
	}

	if(const auto OwnerPlayer = Cast<ISearchOutEffectable>(ActorToAnnounceEnd))
	{
		OwnerPlayer->AnnounceToOwnerEnd();
	}

	Destroy();
}