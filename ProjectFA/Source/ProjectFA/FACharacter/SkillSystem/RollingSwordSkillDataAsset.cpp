// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingSwordSkillDataAsset.h"

void URollingSwordSkillDataAsset::DoSkill()
{
	Super::DoSkill();
}

void URollingSwordSkillDataAsset::SpawnSkillObject(FVector SpawnPosition)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetSkillInstigatorController()->GetPawn();
	GetWorld()->SpawnActor<AActor>(BombClass, SpawnParameters);
}