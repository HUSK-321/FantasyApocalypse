// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootingBox.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class ULootingItemComponent;

UCLASS()
class PROJECTFA_API ALootingBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ALootingBox();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BoxMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> BoxArea;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> ItemGeneratePosition;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ULootingItemComponent> LootingItemComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APickupItem> ItemToTestClass;
	FTimerHandle TestItemTimerHandle;

	UFUNCTION()
	void CallLooting();
};
