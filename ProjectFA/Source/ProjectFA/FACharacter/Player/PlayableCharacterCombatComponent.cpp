// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacterCombatComponent.h"
#include "PlayableController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "ProjectFA/FACharacter/SkillSystem/SkillDataAsset.h"
#include "ProjectFA/FADictionary/GamePlayCalculator.h"
#include "ProjectFA/InGameItem/Weapon/Weapon.h"

UPlayableCharacterCombatComponent::UPlayableCharacterCombatComponent()
	:
	HandSlotCount(2),
	bNowAttacking(false), bDoNextAttack(false)
{
	PrimaryComponentTick.bCanEverTick = false;

	HandSlots.SetNum(HandSlotCount);
}

void UPlayableCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AFACharacter>(GetOwner());
	CreateDefaultWeapon();
	
	CreateSkillFromData();
}

void UPlayableCharacterCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayableCharacterCombatComponent, EquippedItem);
	DOREPLIFETIME(UPlayableCharacterCombatComponent, SkillSlotQ);
	DOREPLIFETIME(UPlayableCharacterCombatComponent, SkillSlotE);
	DOREPLIFETIME(UPlayableCharacterCombatComponent, HandSlots);
}

void UPlayableCharacterCombatComponent::CreateDefaultWeapon()
{
	if(DefaultPunchWeaponClass)
	{
		DefaultPunchWeapon = GetWorld()->SpawnActor<APickupItem>(DefaultPunchWeaponClass);

		if(DefaultPunchWeapon)
		{
			EquipItemToCharacter(DefaultPunchWeapon);
		}
	}
}

void UPlayableCharacterCombatComponent::CreateSkillFromData()
{
	if(SkillSlotQClass)
	{
		SkillSlotQ = NewObject<USkillDataAsset>(this, SkillSlotQClass);
	}
	if(SkillSlotEClass)
	{
		SkillSlotE = NewObject<USkillDataAsset>(this, SkillSlotEClass);
	}

	if(Character->GetController() == nullptr)	return;
	const auto PlayableController = Cast<APlayableController>(Character->GetController());
	if(PlayableController == nullptr)	return;

	// TODO : refactor
	PlayableController->InitializeSkillWidget(GetSkillSlotQ(), GetSkillSlotE());
	if(SkillSlotQ)
	{
		SkillSlotQ->Rename(TEXT("SkillQ"), Character);
		SkillSlotQ->SkillMontageEndEvent.AddDynamic(this, &UPlayableCharacterCombatComponent::DoingSkillEnd);
	}
	if(SkillSlotE)
	{
		SkillSlotE->Rename(TEXT("SkillE"), Character);
		SkillSlotE->SkillMontageEndEvent.AddDynamic(this, &UPlayableCharacterCombatComponent::DoingSkillEnd);
	}
}

void UPlayableCharacterCombatComponent::EquipItemToCharacter(APickupItem* ItemToEquip)
{
	if(Character == nullptr || ItemToEquip == EquippedItem)	return;
	if(const auto Equipable = Cast<IEquipable>(EquippedItem))
	{
		Equipable->UnEquip();
	}
	
	EquippedItem = ItemToEquip;
	EquippedItem->SetOwner(Character);
	EquippedItem->SetItemState(EItemState::EIS_Equipped);
	EquippedItem->ItemDroppedEvent.AddDynamic(this, &UPlayableCharacterCombatComponent::WeaponDrop);
	if(const auto RightHandSocket = Character->GetMesh()->GetSocketByName("hand_r_weapon_socket"))
	{
		RightHandSocket->AttachActor(EquippedItem, Character->GetMesh());
	}
	if(const auto Equipable = Cast<IEquipable>(ItemToEquip))
	{
		FEquipItemEvent Event;
		Event.AddDynamic(this, &UPlayableCharacterCombatComponent::WeaponUnEquip);
		Equipable->SetUnEquipEvent(Event);

		FGetPlayerDamagePropertyDelegate PlayerDamagePropertyDelegate;
		PlayerDamagePropertyDelegate.BindUFunction(this, FName("GetCharacterAttackDamage"));
		Equipable->SetPlayerDamagePropertyDelegate(PlayerDamagePropertyDelegate);
	}
	ManageHandSlots();
}

void UPlayableCharacterCombatComponent::OnRep_EquippedItem()
{
	if(Character == nullptr || EquippedItem == nullptr)	return;
	
	EquippedItem->SetItemState(EItemState::EIS_Equipped);
	if(const auto RightHandSocket = Character->GetMesh()->GetSocketByName("hand_r_weapon_socket"))
	{
		RightHandSocket->AttachActor(EquippedItem, Character->GetMesh());
	}
	
	OnPlayerHandItemChanged.Broadcast(EquippedItem);
}

void UPlayableCharacterCombatComponent::Attack()
{
	if(EquippedItem == nullptr || bNowDoingSkill)	return;
	ServerAttack();
}

void UPlayableCharacterCombatComponent::ServerAttack_Implementation()
{
	MulticastAttack();
}

void UPlayableCharacterCombatComponent::MulticastAttack_Implementation()
{
	if(EquippedItem == nullptr || Character == nullptr || bNowDoingSkill)	return;
	if(bNowAttacking)
	{
		bDoNextAttack = true;
		return;
	}
	if(auto const WeaponInterface = Cast<IEquipable>(EquippedItem))
	{
		bNowAttacking = true;
		TurnToNearbyTarget();
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
		TurnToNearbyTarget();
		return;
	}
	EndNormalAttack();
}

void UPlayableCharacterCombatComponent::EndNormalAttack()
{
	bNowAttacking = false;
	Character->StopNormalAttackMontage();
}

void UPlayableCharacterCombatComponent::PressQButton()
{
	ServerDoSkill(true);
}

void UPlayableCharacterCombatComponent::PressEButton()
{
	ServerDoSkill(false);
}

void UPlayableCharacterCombatComponent::DoSkill(USkillDataAsset* SkillToDo)
{
	if(SkillToDo == nullptr)	return;
	
	TurnToNearbyTarget();
	bNowDoingSkill = true;
	SkillToDo->SetSkillOwnerCharacter(Character);
	SkillToDo->DoSkill();
}

void UPlayableCharacterCombatComponent::ServerDoSkill_Implementation(bool bIsQ)
{
	MulticastDoSkill(bIsQ);
}

void UPlayableCharacterCombatComponent::MulticastDoSkill_Implementation(bool bIsQ)
{
	bIsQ ? DoSkill(GetSkillSlotQ()) : DoSkill(GetSkillSlotE());
}

void UPlayableCharacterCombatComponent::WeaponUnEquip(APickupItem* UnEquipItem)
{
	if(UnEquipItem == EquippedItem)
	{
		EquippedItem = nullptr;
	}
}

void UPlayableCharacterCombatComponent::WeaponDrop(APickupItem* UnEquipItem)
{
	for(int8 i = 0; i < HandSlotCount; i++)
	{
		if(HandSlots[i] != UnEquipItem)	continue;
		
		HandSlots[i] = nullptr;
		if(CurrentSlotIndex == i)
		{
			EquipItemToCharacter(DefaultPunchWeapon);
			SetCurrentSlotIndex(HandSlotCount);
		}
		return;
	}
}

float UPlayableCharacterCombatComponent::GetCharacterAttackDamage() const
{
	// TODO : 플레이어 스탯을 저장하는 것들이 생긴다면 처리하기
	const float ReturnDamage = 5.0f;
	return UGamePlayCalculator::GetCharacterAttackAmplify(ReturnDamage, GetSkillDamageAmplify());
}

float UPlayableCharacterCombatComponent::GetSkillDamageAmplify() const
{
	float ReturnDamage = 0.f;
	if(GetNowDoingSkill())
	{
		if(SkillSlotQ && SkillSlotQ->GetNowPlayingMontage())
		{
			ReturnDamage += SkillSlotQ->GetDamageAmplify();
		}
		if(SkillSlotE && SkillSlotE->GetNowPlayingMontage())
		{
			ReturnDamage += SkillSlotE->GetDamageAmplify();
		}
	}
	return ReturnDamage;
}

void UPlayableCharacterCombatComponent::DoingSkillEnd()
{
	bNowDoingSkill = false;
	bNowAttacking = false;
	bDoNextAttack = false;
}

void UPlayableCharacterCombatComponent::SwapHandSlotWeapon(int8 SlotIndex)
{
	if(bNowAttacking || bNowDoingSkill)	return;
	SetCurrentSlotIndex(SlotIndex);
	ServerSwapWeapon(SlotIndex);
}

void UPlayableCharacterCombatComponent::SetCurrentSlotIndex(int8 NewSlotIndex)
{
	CurrentSlotIndex = NewSlotIndex;
	
	(HandSlots.IsValidIndex(CurrentSlotIndex)) ? 
		OnPlayerHandItemChanged.Broadcast(HandSlots[CurrentSlotIndex]) :
		OnPlayerHandItemChanged.Broadcast(DefaultPunchWeapon);
}

void UPlayableCharacterCombatComponent::ServerSwapWeapon_Implementation(int8 SlotIndex)
{
	if(HandSlots.IsValidIndex(SlotIndex) == false || HandSlots[SlotIndex] == nullptr)
	{
		EquipItemToCharacter(DefaultPunchWeapon);
		SetCurrentSlotIndex(HandSlotCount);
		return;
	}
	EquipItemToCharacter(HandSlots[SlotIndex]);
}

void UPlayableCharacterCombatComponent::ManageHandSlots()
{
	// if default weapon set index to 'HandSlotCount'
	if(EquippedItem == DefaultPunchWeapon)
	{
		SetCurrentSlotIndex(HandSlotCount);
		return;
	}
	// Find already in slot 
	for(int8 SlotIndex = 0; SlotIndex < HandSlotCount; SlotIndex++)
	{
		if(HandSlots[SlotIndex] != EquippedItem)	continue;
		
		SetCurrentSlotIndex(SlotIndex);
		return;
	}
	// Find empty slot
	for(int8 SlotIndex = 0; SlotIndex < HandSlotCount; SlotIndex++)
	{
		if(HandSlots[SlotIndex] != nullptr)	continue;

		HandSlots[SlotIndex] = EquippedItem;
		SetCurrentSlotIndex(SlotIndex);
		return;
	}
	// Not in slot, No empty slot
	if(HandSlots.IsValidIndex(CurrentSlotIndex))
	{
		HandSlots[CurrentSlotIndex] = EquippedItem;
	}
	else
	{
		HandSlots[0] = EquippedItem;
		SetCurrentSlotIndex(0);
	}
}

void UPlayableCharacterCombatComponent::TurnToNearbyTarget()
{
	if(GetOwner() == nullptr)	return;
	
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) };
	const TArray<AActor*> ActorToIgnore { GetOwner() };
	TArray<AActor*> OutActors;
	constexpr float SearchRadius{ 150.f };
	
	UKismetSystemLibrary::SphereOverlapActors(GetOwner(), GetOwner()->GetActorLocation(), SearchRadius, ObjectTypes,
												AFACharacter::StaticClass(), ActorToIgnore, OutActors);
	for(auto HitActor : OutActors)
	{
		const auto FACharacter = Cast<AFACharacter>(HitActor);
		if(FACharacter == nullptr)	continue;
		
		auto LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), FACharacter->GetActorLocation());
		LookAtRotation.Pitch = 0.f;
		GetOwner()->SetActorRotation(LookAtRotation);
		UKismetSystemLibrary::DrawDebugSphere(GetOwner(), GetOwner()->GetActorLocation(), SearchRadius, 30, FLinearColor::Green, .5f, 1.f);
		return;
	}
	UKismetSystemLibrary::DrawDebugSphere(GetOwner(), GetOwner()->GetActorLocation(), SearchRadius, 30, FLinearColor::Blue, .5f, 1.f);
}