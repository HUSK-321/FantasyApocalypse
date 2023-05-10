// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "PickupItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UItemDataAsset;

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Initial		UMETA(DisplayName = "Initial State"),
	EIS_Equipped	UMETA(DisplayName = "Equipped"),
	EIS_Dropped		UMETA(DisplayName = "Dropped"),
	EIS_InInventory	UMETA(DisplayName = "InInventory"),

	EIS_MAX			UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemDataAsset* ItemDataAsset;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemDroppedEvent, APickupItem*, Item);

UCLASS()
class PROJECTFA_API APickupItem : public AActor
{
	GENERATED_BODY()

public:
	FItemDroppedEvent ItemDroppedEvent;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> PickupItemMesh;
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
	UPROPERTY(ReplicatedUsing = OnRep_ItemState, VisibleAnywhere, Category = "Item Property")
	EItemState ItemState;

	FTimerHandle DropTimer;
	
public:
	APickupItem();
	virtual void SetItemPropertyFromDataAsset(const UItemDataAsset* DataAsset);

	void DropItem(const float DropImpulsePower = 5000.f);
	void SetItemState(const EItemState State);
	virtual void SetItemVisibilityByState();
	
	virtual void SetOwner(AActor* NewOwner) override;

	FORCEINLINE FString GetItemName() const { return ItemName; }
	FORCEINLINE FString GetItemDescription() const { return ItemDescription; }
	FORCEINLINE float GetItemWeight() const { return ItemWeight; }
	FORCEINLINE UTexture2D* GetItemIcon() const { return ItemIcon; }

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void PickupAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void PickupAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void DropEnd();

	UFUNCTION()
	void OnRep_ItemState();
};