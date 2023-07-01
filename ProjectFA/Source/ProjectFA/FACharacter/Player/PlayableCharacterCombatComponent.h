// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectFA/FAInterfaces/WeaponAttackableComponent.h"
#include "PlayableCharacterCombatComponent.generated.h"


class APlayableCharacter;
class AFACharacter;
class AWeapon;
class UEquipable;
class APickupItem;
class USkillDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerHandItemChanged, APickupItem*, Item);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFA_API UPlayableCharacterCombatComponent : public UActorComponent, public IWeaponAttackableComponent
{
	GENERATED_BODY()

public:
	FOnPlayerHandItemChanged OnPlayerHandItemChanged;

private:
	UPROPERTY()
	TObjectPtr<AFACharacter> Character;
	UPROPERTY(ReplicatedUsing = OnRep_EquippedItem, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APickupItem> EquippedItem;
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APickupItem> DefaultPunchWeaponClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APickupItem> DefaultPunchWeapon;

	UPROPERTY(VisibleAnywhere)
	int8 CurrentSlotIndex;
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<APickupItem*> HandSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USkillDataAsset> SkillSlotQClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USkillDataAsset> SkillSlotEClass;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkillDataAsset> SkillSlotQ;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkillDataAsset> SkillSlotE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bNowAttacking;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bDoNextAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bNowDoingSkill;

public:
	UPlayableCharacterCombatComponent();

	virtual void WeaponAttackStart() override;
	virtual void WeaponAttackEnd() override;

	void EquipItemToCharacter(APickupItem* ItemToEquip);

	void Attack();
	UFUNCTION(Server, Reliable)
	void ServerAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack();
	
	UFUNCTION(BlueprintCallable)
	void CheckShouldStopAttack();
	UFUNCTION(BlueprintCallable)
	void EndNormalAttack();

	UFUNCTION()
	void PressQButton();
	UFUNCTION()
	void PressEButton();
	UFUNCTION()
	void DoSkill(USkillDataAsset* SkillToDo);
	UFUNCTION(Server, Reliable)
	void ServerDoSkill(bool bIsQ);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDoSkill(bool bIsQ);

	void SwapHandSlotWeapon(int8 SlotIndex);

	FORCEINLINE bool GetNowAttacking() const { return bNowAttacking; }
	FORCEINLINE bool GetNowDoingSkill() const { return bNowDoingSkill; }
	FORCEINLINE void SetSkillSlotQ(USkillDataAsset* SkillDataAsset) { SkillSlotQ = SkillDataAsset; }
	FORCEINLINE void SetSkillSlotE(USkillDataAsset* SkillDataAsset) { SkillSlotE = SkillDataAsset; }
	FORCEINLINE USkillDataAsset* GetSkillSlotQ() const { return SkillSlotQ; }
	FORCEINLINE USkillDataAsset* GetSkillSlotE() const { return SkillSlotE; }
	UFUNCTION()
	float GetCharacterAttackDamage() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void CreateDefaultWeapon();
	void CreateSkillFromData();
	void TurnToNearbyTarget();
	
	UFUNCTION()
	void WeaponUnEquip(APickupItem* UnEquipItem);
	UFUNCTION()
	void WeaponDrop(APickupItem* UnEquipItem);
	float GetSkillDamageAmplify() const;

	UFUNCTION()
	void OnRep_EquippedItem();

	UFUNCTION(BlueprintCallable)
	void DoingSkillEnd();

	void SetCurrentSlotIndex(int8 NewSlotIndex);

	UFUNCTION(Server, Reliable)
	void ServerSwapWeapon(int8 SlotIndex);
	
	void FillHandSlots();
};