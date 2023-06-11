// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataAsset.h"
#include "GameFramework/Character.h"

USkillDataAsset::USkillDataAsset()
	:
	bNowCooldown(false)
{
}

void USkillDataAsset::DoSkill()
{
	if(bNowCooldown)	return;
	
	bNowCooldown = true;
	if(GetWorld())
	{
		SkillCoolTimeStartEvent.Broadcast();
		GetWorld()->GetTimerManager().SetTimer(SkillTimerHandle, this, &USkillDataAsset::ResetSkill, CoolTime);
		GetWorld()->GetTimerManager().SetTimer(SkillCoolTimeHandle, this, &USkillDataAsset::UpdateCoolTime, 0.1f, true, 0.f);
	}
	PlaySkillMontage();
}

void USkillDataAsset::PlaySkillMontage()
{
	if(GetSkillOwner() == nullptr)	return;
	const auto CharacterMesh = GetSkillOwner()->GetMesh();
	if(CharacterMesh == nullptr)	return;
	const auto CharacterAnimInstance = CharacterMesh->GetAnimInstance();
	if(CharacterAnimInstance == nullptr)	return;
	
	CharacterAnimInstance->StopAllMontages(0.0f);
	CharacterAnimInstance->Montage_Play(SkillMontage);
	CharacterAnimInstance->OnMontageEnded.Clear();
	CharacterAnimInstance->OnMontageEnded.AddDynamic(this, &USkillDataAsset::SkillMontageEnd);
	bNowPlayingMontage = true;
}

void USkillDataAsset::ResetSkill()
{
	bNowCooldown = false;
	SkillCoolTimeEndEvent.Broadcast();
}

void USkillDataAsset::UpdateCoolTime()
{
	const auto RemainTime = GetNowCoolTime();
	if(GetNowCoolTime() < KINDA_SMALL_NUMBER && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SkillCoolTimeHandle);
		bNowCooldown = false;
	}
	SkillDoingEvent.Broadcast(RemainTime);
}

void USkillDataAsset::SkillMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bNowPlayingMontage = false;
	SkillMontageEndEvent.Broadcast();
}

float USkillDataAsset::GetNowCoolTime() const
{
	return (GetWorld() != nullptr && bNowCooldown) ? GetWorld()->GetTimerManager().GetTimerRemaining(SkillTimerHandle) : 0.f;	
}
