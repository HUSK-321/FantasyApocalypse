// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SetMeshVisualAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFA_API USetMeshVisualAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	USetMeshVisualAnimNotify();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Visual", meta = (AllowPrivateAccess = "true"))
	uint8 bMeshActive : 1;
};