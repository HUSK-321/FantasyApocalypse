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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemDroppedEvent, APickupItem*, Item);

UCLASS()
class PROJECTFA_API APickupItem : public AActor
{
	GENERATED_BODY()

public:
	FItemDroppedEvent ItemDroppedEvent;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> PickupItemMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> PickupAreaSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Property")
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Property")
	FString ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Property")
	TObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditAnywhere, Category = "Item Property")
	float ItemPowerAmount;
	UPROPERTY(EditAnywhere, Category = "Item Property")
	float ItemWeight;
	UPROPERTY()
	EItemState ItemState;

	FTimerHandle DropTimer;
	
public:
	APickupItem();
	
	void DropItem();
	void SetItemState(const EItemState State);
	
	virtual void SetOwner(AActor* NewOwner) override;

	FORCEINLINE FString GetItemName() const { return ItemName; }
	FORCEINLINE FString GetItemDescription() const { return ItemDescription; }
	FORCEINLINE float GetItemWeight() const { return ItemWeight; }
	FORCEINLINE UTexture2D* GetItemIcon() const { return ItemIcon; }

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:
	UFUNCTION()
	void PickupAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void PickupAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void DropEnd();
};
