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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Search Out", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> SearchOutList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search Out", meta = (AllowPrivateAccess = "true"))
	float SearchOutTime;
	FTimerHandle SearchOutTimer;
	FTimerHandle SearchOutTimerServer;

	UPROPERTY()
	TObjectPtr<AActor> ActorToAnnounceEnd;

public:
	ASearchOutItem();
	
	virtual void InventoryAction_Implementation() override;
	virtual void RemoveFromInventoryAction_Implementation() override;
	virtual void UseAction() override;
	
	UFUNCTION(Client, Reliable)
	void ClientDoSearchOut();
	UFUNCTION()
	void DoSearchOut();
	
private:
	void ResetSearchOutActors();
	UFUNCTION(Client, Reliable)
	void ClientResetSearchOut();
	
	void EnableSearchOutEffect();
	void AnnounceDetectedToPlayers();
	void DisableSearchOutEffect();
	void SearchOutByOverlap();
};
