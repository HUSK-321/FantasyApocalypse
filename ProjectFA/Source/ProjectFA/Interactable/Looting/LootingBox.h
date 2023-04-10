// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LootableActor.h"
#include "GameFramework/Actor.h"
#include "LootingBox.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class ULootingItemComponent;

UCLASS()
class PROJECTFA_API ALootingBox : public AActor, public ILootableActor
{
	GENERATED_BODY()
	
public:	
	ALootingBox();

	virtual void SearchActor(float SearchTime) override;
	virtual void FindItem_Implementation(const float SearchTime) override;

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

	UPROPERTY(EditAnywhere, Category = "Looting Property")
	float TimeToSearch;

private:
	UFUNCTION()
	void LootAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void LootAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
