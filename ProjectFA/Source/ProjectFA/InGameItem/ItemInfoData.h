// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponType.h"
#include "UObject/NoExportTypes.h"
#include "ItemInfoData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPickupItemInfoData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FString ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	float ItemPowerAmount;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	float ItemWeight;
};

USTRUCT(BlueprintType)
struct FWeaponItemInfoData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Weapon Data")
	TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Data")
	EWeaponType WeaponType;
};



UCLASS()
class PROJECTFA_API UItemInfoData : public UObject
{
	GENERATED_BODY()
	
};
