#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Default			UMETA(DisplayName = "Default"),
	EWT_OneHandSword	UMETA(DisplayName = "One Hand Sword"),
	EWT_MagicStaff		UMETA(DisplayName = "Magic Staff"),

	EWT_MAX				UMETA(DisplayName = "DefaultMAX")
};
