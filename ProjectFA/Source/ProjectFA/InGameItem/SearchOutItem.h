// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryUsable.h"
#include "PickupItem.h"
#include "SearchOutItem.generated.h"


UCLASS()
class PROJECTFA_API ASearchOutItem : public APickupItem, public IInventoryUsable
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<AActor*> SearchOutList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search Out", meta = (AllowPrivateAccess = "true"))
	float SearchOutTime;
	FTimerHandle SearchOutTimer;

public:
	ASearchOutItem();
	
	virtual void InventoryAction_Implementation() override;
	virtual void RemoveFromInventoryAction_Implementation() override;
	virtual void UseAction() override;
private:
	void ResetSearchOutActors();
};
