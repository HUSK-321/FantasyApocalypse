// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectFA/FAInterfaces/LootableComponent.h"
#include "LootingItemComponent.generated.h"

class APickupItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFA_API ULootingItemComponent : public UActorComponent, public ILootableComponent
{
	GENERATED_BODY()

public:	
	ULootingItemComponent();
	void InitializeItemList(const TArray<APickupItem*>& List);

	virtual void GenerateItemsToWorld() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Looting")
	TArray<TObjectPtr<APickupItem>> ItemList;
};
