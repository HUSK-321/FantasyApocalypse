// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacterCombatComponent.h"
#include "PlayableController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "ProjectFA/FACharacter/SkillSystem/SkillDataAsset.h"
#include "ProjectFA/InGameItem/Weapon/Weapon.h"

UPlayableCharacterCombatComponent::UPlayableCharacterCombatComponent()
	:
	bNowAttacking(false), bDoNextAttack(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayableCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AFACharacter>(GetOwner());
	if(DefaultPunchWeaponClass)
	{
		DefaultPunchWeapon = GetWorld()->SpawnActor<APickupItem>(DefaultPunchWeaponClass);
		EquipItemToCharacter(DefaultPunchWeapon);
	}

	const auto CharacterController = Cast<APlayerController>(Character->GetController());
	if(CharacterController == nullptr)	return;
	const auto PlayableController = Cast<APlayableController>(CharacterController);
	if(PlayableController == nullptr)	return;

	PlayableController->InitializeSkillWidget(GetSkillSlotQ(), GetSkillSlotE());
	// TODO : refactor
	if(SkillSlotQ)
	{
		SkillSlotQ->Rename(TEXT("SkillQ"), Character);
		SkillSlotQ->SkillCoolTimeStartEvent.AddDynamic(PlayableController, &APlayableController::SetSkillQCoolTimeVisible);
		SkillSlotQ->SkillCoolTimeEndEvent.AddDynamic(PlayableController, &APlayableController::SetSkillQCoolTimeHidden);
	}
	if(SkillSlotE)
	{
		SkillSlotE->Rename(TEXT("SkillE"), Character);
		SkillSlotE->SkillCoolTimeStartEvent.AddDynamic(PlayableController, &APlayableController::SetSkillECoolTimeVisible);
		SkillSlotE->SkillCoolTimeEndEvent.AddDynamic(PlayableController, &APlayableController::SetSkillECoolTimeHidden);
	}
}

void UPlayableCharacterCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayableCharacterCombatComponent, EquippedItem);
}

void UPlayableCharacterCombatComponent::EquipItemToCharacter(APickupItem* ItemToEquip)
{
	if(Character == nullptr)	return;
	if(const auto Equipable = Cast<IEquipable>(EquippedItem))
	{
		Equipable->UnEquip();
	}
	EquippedItem = ItemToEquip;
	EquippedItem->SetOwner(Character);
	EquippedItem->SetItemState(EItemState::EIS_Equipped);
	if(const auto RightHandSocket = Character->GetMesh()->GetSocketByName("hand_r_weapon_socket"))
	{
		RightHandSocket->AttachActor(EquippedItem, Character->GetMesh());
	}
	if(const auto Equipable = Cast<IEquipable>(ItemToEquip))
	{
		FEquipItemEvent Event;
		Event.AddDynamic(this, &UPlayableCharacterCombatComponent::ItemDrop);
		Equipable->SetUnEquipEvent(Event);
	}
}

void UPlayableCharacterCombatComponent::OnRep_EquippedItem()
{
	if(Character == nullptr || EquippedItem == nullptr)	return;
	
	EquippedItem->SetItemState(EItemState::EIS_Equipped);
	if(const auto RightHandSocket = Character->GetMesh()->GetSocketByName("hand_r_weapon_socket"))
	{
		RightHandSocket->AttachActor(EquippedItem, Character->GetMesh());
	}
}

void UPlayableCharacterCombatComponent::Attack()
{
	ServerAttack();
}

void UPlayableCharacterCombatComponent::ServerAttack_Implementation()
{
	MulticastAttack();
}

void UPlayableCharacterCombatComponent::MulticastAttack_Implementation()
{
	if(EquippedItem == nullptr || Character == nullptr)	return;
	if(bNowAttacking)
	{
		bDoNextAttack = true;
		return;
	}
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		bNowAttacking = true;
		Character->PlayNormalAttackMontage(WeaponInterface->GetNormalAttackMontageSectionName());
	}
}

void UPlayableCharacterCombatComponent::WeaponAttackStart()
{
	if(EquippedItem == nullptr)	return;
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		WeaponInterface->AttackStart_Implementation();
	}
}

void UPlayableCharacterCombatComponent::WeaponAttackEnd()
{
	if(EquippedItem == nullptr)	return;
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		WeaponInterface->AttackEnd_Implementation();
		bDoNextAttack = false;
	}
}

void UPlayableCharacterCombatComponent::CheckShouldStopAttack()
{
	if(bDoNextAttack || Character == nullptr)
	{
		bDoNextAttack = false;
		return;
	}
	EndAttack();
}

void UPlayableCharacterCombatComponent::EndAttack()
{
	bNowAttacking = false;
	Character->StopNormalAttackMontage();
}

void UPlayableCharacterCombatComponent::PressQButton()
{
	if(SkillSlotQ == nullptr)	return;
	const auto CharacterController = Cast<APlayerController>(Character->GetController());
	SkillSlotQ->SetSkillInstigatorController(CharacterController);
	SkillSlotQ->DoSkill();
}

void UPlayableCharacterCombatComponent::PressEButton()
{
	if(SkillSlotE == nullptr)	return;
	const auto CharacterController = Cast<APlayerController>(Character->GetController());
	SkillSlotE->SetSkillInstigatorController(CharacterController);
	SkillSlotE->DoSkill();
}

void UPlayableCharacterCombatComponent::ItemDrop(APickupItem* UnEquipItem)
{
	if(UnEquipItem == EquippedItem)
	{
		EquippedItem = nullptr;
	}
}

float UPlayableCharacterCombatComponent::GetCharacterAttackDamage()
{
	if(EquippedItem == nullptr)	return 0.f;
	return EquippedItem->GetItemPower();
}