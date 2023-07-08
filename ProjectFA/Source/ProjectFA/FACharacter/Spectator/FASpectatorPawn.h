// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "FASpectatorPawn.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class PROJECTFA_API AFASpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> SpectorBasicMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> NextPlayerAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PrevPlayerAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EscapeAction;

public:
	AFASpectatorPawn();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void SpectateNextPlayer();
	UFUNCTION()
	void SpectatePrevPlayer();
	UFUNCTION()
	void EscapeActionPressed();
};
