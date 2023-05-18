// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoveryItem.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectFA/FACharacter/Player/PlayableController.h"

ARecoveryItem::ARecoveryItem()
{
}

void ARecoveryItem::InventoryAction_Implementation()
{
	if(GetOwner()->GetInstigatorController() == nullptr)	return;
	UGameplayStatics::ApplyDamage(GetOwner(), -ItemInfo.ItemPowerAmount, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
	Destroy();
}

void ARecoveryItem::RemoveFromInventoryAction_Implementation()
{
	const auto OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr)	return;
	if(const auto OwnerController = OwnerPawn->GetController<APlayableController>())
	{
		OwnerController->ServerDropItem(this);
	}
}