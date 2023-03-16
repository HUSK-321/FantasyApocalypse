// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectFA/FACharacter/FACharacter.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;

UCLASS()
class PROJECTFA_API AEnemy : public AFACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;
	TObjectPtr<AActor> TargetActor;

public:
	
	AEnemy();

protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSensingPawn(APawn* OtherPawn);
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser) override;
};
