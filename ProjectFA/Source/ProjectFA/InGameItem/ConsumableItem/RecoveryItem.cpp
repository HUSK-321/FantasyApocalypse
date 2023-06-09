// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoveryItem.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectFA/FAInterfaces/Controller/ItemRPCableController.h"

ARecoveryItem::ARecoveryItem()
{
}

void ARecoveryItem::InventoryAction_Implementation()
{
	RequestControllerUseItem();
}

void ARecoveryItem::RemoveFromInventoryAction_Implementation()
{
	const auto OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr)	return;
	if(const auto OwnerController = OwnerPawn->GetController<IItemRPCableController>())
	{
		OwnerController->DropItem(this);
	}
}

void ARecoveryItem::UseAction()
{
	if(GetOwner()->GetInstigatorController() == nullptr)	return;
	UGameplayStatics::ApplyDamage(GetOwner(), -ItemInfo.ItemPowerAmount, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
	Destroy();
}

void ARecoveryItem::RequestControllerUseItem()
{
	const auto PlayerController = GetGameInstance()->GetFirstLocalPlayerController(GetWorld());
	if(const auto Controller = Cast<IItemRPCableController>(PlayerController))
	{
		Controller->UseItem(this);
	}
}