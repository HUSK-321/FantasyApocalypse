// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataAsset.h"
#include "GameFramework/Character.h"

void USkillDataAsset::DoSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("do skill"));
	PlaySkillMontage();
}

void USkillDataAsset::PlaySkillMontage()
{
	if(GetSkillInstigatorController() == nullptr)	return;
	const auto Character = Cast<ACharacter>(GetSkillInstigatorController()->GetPawn());
	if(Character == nullptr)	return;
	const auto CharacterMesh = Character->GetMesh();
	if(CharacterMesh == nullptr)	return;
	const auto CharacterAnimInstance = CharacterMesh->GetAnimInstance();
	if(CharacterAnimInstance == nullptr)	return;
	
	CharacterAnimInstance->StopAllMontages(0.0f);
	CharacterAnimInstance->Montage_Play(SkillMontage);
}