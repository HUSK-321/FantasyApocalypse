// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableWithCharacter.generated.h"

UINTERFACE(MinimalAPI)
class UInteractableWithCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTFA_API IInteractableWithCharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Looting Action")
	void InteractWithObject(const float InteractTime);

	virtual void EndInteracting(){}
};
