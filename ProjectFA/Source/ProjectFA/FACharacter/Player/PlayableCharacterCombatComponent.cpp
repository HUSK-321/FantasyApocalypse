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
	
	CreateSkillFromData();
}

void UPlayableCharacterCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayableCharacterCombatComponent, EquippedItem);
	DOREPLIFETIME(UPlayableCharacterCombatComponent, SkillSlotQ);
	DOREPLIFETIME(UPlayableCharacterCombatComponent, SkillSlotE);
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

	const auto CharacterController = Cast<APlayerController>(Character->GetController());
	if(CharacterController == nullptr)	return;
	const auto PlayableController = Cast<APlayableController>(CharacterController);
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

		FGetPlayerDamagePropertyDelegate PlayerDamagePropertyDelegate;
		PlayerDamagePropertyDelegate.BindUFunction(this, FName("GetCharacterAttackDamage"));
		Equipable->SetPlayerDamagePropertyDelegate(PlayerDamagePropertyDelegate);
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
	if(EquippedItem == nullptr)	return;
	TurnToNearbyTarget();
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
	const auto CharacterController = Cast<APlayerController>(Character->GetController());
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

void UPlayableCharacterCombatComponent::ItemDrop(APickupItem* UnEquipItem)
{
	if(UnEquipItem == EquippedItem)
	{
		EquippedItem = nullptr;
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
}

void UPlayableCharacterCombatComponent::TurnToNearbyTarget()
{
	TArray<FHitResult> HitResults;
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic) };
	const TArray<AActor*> ActorToIgnore;
	const FVector HalfSize{ 60.f, 60.f, 10.f};
	UKismetSystemLibrary::BoxTraceMultiForObjects(this, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation(), HalfSize, FRotator::ZeroRotator,
													ObjectTypes, false, ActorToIgnore, EDrawDebugTrace::Persistent, HitResults, true);
	
	for(auto HitResult : HitResults)
	{
		auto FACharacter = Cast<AFACharacter>(HitResult.GetActor());
		if(FACharacter == nullptr)	continue;
	
		auto LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), FACharacter->GetActorLocation());
		LookAtRotation.Pitch = 0.f;
		GetOwner()->SetActorRotation(LookAtRotation);
		return;
	}
}