// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayCalculator.h"

float UGamePlayCalculator::GetCharacterAttackAmplify(const float& CharacterAttackProperty, const float& SkillAmplify)
{
	float ReturnAttackValue = CharacterAttackProperty;
	ReturnAttackValue *= (1.f + (SkillAmplify/100.f));
	
	return ReturnAttackValue;
}

float UGamePlayCalculator::CalculateWeaponDamage(const float& WeaponDamage, const float& PlayerAmplify)
{
	float ReturnWeaponDamage = WeaponDamage;
	ReturnWeaponDamage *= (1.f + (PlayerAmplify/100.f));

	return ReturnWeaponDamage;
}
