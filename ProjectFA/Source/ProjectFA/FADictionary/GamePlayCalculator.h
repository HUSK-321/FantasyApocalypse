// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GamePlayCalculator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFA_API UGamePlayCalculator : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * @brief Calculate character's final attack amount.
	 *
	 * @param CharacterAttackProperty	Character's whole attack property (base attack property, buff, other etc)
	 * @param SkillAmplify				Type Skill's amplify amount, if now doing skill
	 */
	UFUNCTION()
	static float GetCharacterAttackAmplify(const float& CharacterAttackProperty, const float& SkillAmplify = 0.f);

	/**
	 * @brief Calculate weapon's final attack amount
	 * 
	 * @param WeaponDamage		Weapon's item power
	 * @param PlayerAmplify		Player's attack amplify. (Use UGamePlayCalculator::GetCharacterAttackAmplify)
	 */
	UFUNCTION()
	static float CalculateWeaponDamage(const float& WeaponDamage, const float& PlayerAmplify = 0.f);
};