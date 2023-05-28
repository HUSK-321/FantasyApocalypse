// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"
#include "SkillSlotWidget.h"
#include "ProjectFA/FACharacter/SkillSystem/SkillDataAsset.h"

void USkillWidget::SetSkillSlotWidget(USkillDataAsset* QSkill, USkillDataAsset* ESkill)
{
	if(QSkill)
	{
		SkillSlotQ->SetSkill(QSkill);
	}
	if(ESkill)
	{
		SkillSlotE->SetSkill(ESkill);
	}
}