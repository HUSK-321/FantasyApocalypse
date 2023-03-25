// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Initial		UMETA(DisplayName = "Initial State"),
	EIS_Equipped	UMETA(DisplayName = "Equipped"),
	EIS_Dropped		UMETA(DisplayName = "Dropped"),
	EIS_InInventory	UMETA(DisplayName = "InInventory"),

	EIS_MAX			UMETA(DisplayName = "DefaultMAX")
};

class USkeletalMeshComponent;
class USphereComponent;

UCLASS()
class PROJECTFA_API APickupItem : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> PickupItemMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> PickupAreaSphere;

	UPROPERTY(EditAnywhere, Category = "Item Property")
	FString ItemName;
	UPROPERTY(EditAnywhere, Category = "Item Property")
	FString ItemDescription;
	UPROPERTY(EditAnywhere, Category = "Item Property")
	float ItemPowerAmount;
	UPROPERTY(EditAnywhere, Category = "Item Property")
	float ItemWeight;

	EItemState ItemState;
	
public:
	
	APickupItem();
	void SetItemState(const EItemState State);

	FORCEINLINE FString GetItemName() const { return ItemName; }

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
