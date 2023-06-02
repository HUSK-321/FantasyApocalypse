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

FName UGamePlayCalculator::GetDirectionSectionName(FVector& OriginForwardVector, const FVector& OriginPosition, const FVector& TargetPosition)
{
	FName ReturnSection;
	
	OriginForwardVector.Normalize();
	auto ToTargetVector = TargetPosition - OriginPosition;
	ToTargetVector.Z = 0;
	ToTargetVector.Normalize();

	const auto DotProduct = FVector::DotProduct(OriginForwardVector, ToTargetVector);
	const auto CrossProduct = FVector::CrossProduct(OriginForwardVector, ToTargetVector);
	const auto LR = FVector::DotProduct(CrossProduct, FVector::UpVector);
	const auto AcosAngle = FMath::Acos(DotProduct);
	const auto Angle = FMath::RadiansToDegrees(AcosAngle);

	if(Angle >= 0 && Angle <= 45)
	{
		ReturnSection = TEXT("Front");
	}
	else if (Angle >= 135 && Angle <= 180)
	{
		ReturnSection = TEXT("Back");
	}
	else
	{
		if(LR > 0)
		{
			ReturnSection = TEXT("Right");
		}
		else
		{
			ReturnSection = TEXT("Left");
		}
	}

	return ReturnSection;
}
