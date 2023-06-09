// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillDataAsset.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSkillEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillDoingEvent, const float&, RemainTime);

UCLASS(Blueprintable, BlueprintType)
class PROJECTFA_API USkillDataAsset : public UObject
{
	GENERATED_BODY()

public:
	FSkillEvent SkillCoolTimeStartEvent;
	FSkillDoingEvent SkillDoingEvent;
	FSkillEvent SkillCoolTimeEndEvent;

	FSkillEvent SkillMontageEndEvent;

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

	bool bNowCooldown;
	bool bNowPlayingMontage;
	FTimerHandle SkillTimerHandle;
	FTimerHandle SkillCoolTimeHandle;

	UPROPERTY()
	TObjectPtr<ACharacter> SkillOwnerCharacter;

public:
	USkillDataAsset();

	FORCEINLINE void SetSkillOwnerCharacter(ACharacter* OwnerCharacter) { SkillOwnerCharacter = OwnerCharacter; }
	
	float GetNowCoolTime() const;
	FORCEINLINE ACharacter* GetSkillOwner() const { return SkillOwnerCharacter; }
	FORCEINLINE float GetDamageAmplify() const { return DamageAmplify; }
	FORCEINLINE float GetSkillCoolTime() const { return CoolTime; }
	FORCEINLINE float GetCost() const { return Cost; }
	FORCEINLINE UTexture2D* GetThumbnail() const { return Thumbnail; }
	FORCEINLINE FString GetSkillName() const { return SkillName; }
	FORCEINLINE FString GetSkillDescription() const { return SkillDescription; }
	FORCEINLINE bool GetNowPlayingMontage() const { return bNowPlayingMontage; }

	virtual void DoSkill();

protected:
	void PlaySkillMontage();

private:
	UFUNCTION()
	void ResetSkill();
	UFUNCTION()
	void UpdateCoolTime();
	UFUNCTION()
	void SkillMontageEnd(UAnimMontage* Montage, bool bInterrupted);
};