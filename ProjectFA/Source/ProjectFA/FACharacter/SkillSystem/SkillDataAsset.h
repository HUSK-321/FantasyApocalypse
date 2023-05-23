// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFA_API USkillDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data", meta = (AllowPrivateAccess = "true"))
	float DamageAmplify;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SkillMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data", meta = (AllowPrivateAccess = "true"))
	float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data", meta = (AllowPrivateAccess = "true"))
	float Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture2D> Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data", meta = (AllowPrivateAccess = "true"))
	FString SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data", meta = (AllowPrivateAccess = "true"))
	FString SkillDescription;

	UPROPERTY()
	TObjectPtr<APlayerController> SkillInstigatorController;

public:
	FORCEINLINE void SetSkillInstigatorController(APlayerController* Controller) { SkillInstigatorController = Controller; }
	
	FORCEINLINE float GetDamageAmplify() const { return DamageAmplify; }
	FORCEINLINE float GetCoolTime() const { return CoolTime; }
	FORCEINLINE float GetCost() const { return Cost; }
	FORCEINLINE UTexture2D* GetThumbnail() const { return Thumbnail; }
	FORCEINLINE FString GetSkillName() const { return SkillName; }
	FORCEINLINE FString GetSkillDescription() const { return SkillDescription; }
	FORCEINLINE APlayerController* GetSkillInstigatorController() const { return SkillInstigatorController; }

	virtual void DoSkill();

protected:
	void PlaySkillMontage();
};