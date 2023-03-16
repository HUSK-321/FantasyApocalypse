// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

class USkeletalMeshComponent;
class USphereComponent;

UCLASS()
class PROJECTFA_API APickupItem : public AActor
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> PickupItemMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> PickupAreaSphere;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
	FString ItemName;
	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
	FString ItemDescription;
	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
	float ItemPowerAmount;
	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
	float ItemWeight;
	
public:
	
	APickupItem();

protected:
	
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void PickupAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void PickupAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
