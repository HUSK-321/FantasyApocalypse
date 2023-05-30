// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/InGameItem/Equipable.h"
#include "ProjectFA/InGameItem/InventoryUsable.h"
#include "ProjectFA/InGameItem/PickupItem.h"
#include "ProjectFA/InGameItem/ItemInfoData.h"
#include "Weapon.generated.h"

/**
 * 
 */

class UBoxComponent;

UCLASS()
class PROJECTFA_API AWeapon : public APickupItem, public IEquipable, public IInventoryUsable
{
	GENERATED_BODY()

public:
	FGetPlayerDamagePropertyDelegate GetPlayerDamageProperty;
	
	FEquipItemEvent EquipItemEvent;
	FEquipItemEvent UnEquipEvent;

private:
	UPROPERTY(ReplicatedUsing = OnRep_WeaponSkeletal)
	TObjectPtr<USkeletalMesh> WeaponSkeletal;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Weapon Properties")
	FWeaponItemInfoData WeaponInfo;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TObjectPtr<UBoxComponent> AttackCollision;
	
	UPROPERTY()
	TArray<AActor*> HittedActors;

public:
	AWeapon();
	virtual void SetItemPropertyFromDataAsset(const UItemDataAsset* DataAsset) override;
	virtual void SetItemVisibilityByState() override;

	virtual void DropItem(const float DropImpulsePower) override;
	
	virtual FName GetNormalAttackMontageSectionName() const override;
	virtual void UnEquip() override;
	virtual void AttackStart_Implementation() override;
	virtual void AttackEnd_Implementation() override;
	virtual void SetEquipItemEvent(const FEquipItemEvent& Event) override;
	virtual void SetUnEquipEvent(const FEquipItemEvent& Event) override;
	virtual void SetPlayerDamagePropertyDelegate(const FGetPlayerDamagePropertyDelegate& Event) override;

	virtual void InventoryAction_Implementation() override;
	virtual void RemoveFromInventoryAction_Implementation() override;

	UFUNCTION()
	void AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_WeaponSkeletal();
};