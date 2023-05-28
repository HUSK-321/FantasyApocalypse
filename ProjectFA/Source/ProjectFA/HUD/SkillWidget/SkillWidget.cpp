// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"
#include "SkillSlotWidget.h"
#include "ProjectFA/FACharacter/SkillSystem/SkillDataAsset.h"

void USkillWidget::SetSkillSlotWidget(const USkillDataAsset* QSkill, const USkillDataAsset* ESkill)
{
	if(QSkill)
	{
		const auto QSkillThumbnail = QSkill->GetThumbnail();
		SkillSlotQ->SetSkillImage(QSkillThumbnail);
	}
	if(ESkill)
	{
		const auto ESkillThumbnail = ESkill->GetThumbnail();
		SkillSlotE->SetSkillImage(ESkillThumbnail);
	}
}