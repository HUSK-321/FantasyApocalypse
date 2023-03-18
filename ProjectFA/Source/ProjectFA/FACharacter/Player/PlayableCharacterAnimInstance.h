// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayableCharacterAnimInstance.generated.h"

class APlayableCharacter;
UCLASS()
class PROJECTFA_API UPlayableCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APlayableCharacter> PlayableCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Player Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category = "Player Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching;

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:

	void SetPlayableCharacterProperties();
};
