// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillDataAsset.h"
#include "ProjectFA/FAInterfaces/ISpawnableSkill.h"
#include "RollingSwordSkillDataAsset.generated.h"

UCLASS()
class PROJECTFA_API URollingSwordSkillDataAsset : public USkillDataAsset, public IISpawnableSkill
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BombClass;

public:
	virtual void DoSkill() override;
	virtual void SpawnSkillObject(FVector SpawnPosition) override;
};