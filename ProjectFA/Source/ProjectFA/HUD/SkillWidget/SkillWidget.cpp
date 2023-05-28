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

void USkillWidget::SetSkillQCoolTimeVisible()
{
	if(SkillSlotQ == nullptr)	return;
	SkillSlotQ->SetCooldownWidgetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SetSkillECoolTimeVisible()
{
	if(SkillSlotE == nullptr)	return;
	SkillSlotE->SetCooldownWidgetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SetSkillQCoolTimeHidden()
{
	if(SkillSlotQ == nullptr)	return;
	SkillSlotQ->SetCooldownWidgetVisibility(ESlateVisibility::Hidden);
}

void USkillWidget::SetSkillECoolTimeHidden()
{
	if(SkillSlotE == nullptr)	return;
	SkillSlotE->SetCooldownWidgetVisibility(ESlateVisibility::Hidden);
}

void USkillWidget::SetSkillQTime(const float RemainTime)
{
	if(SkillSlotQ == nullptr)	return;
	SkillSlotQ->SetSkillCoolTimeText(RemainTime);
}

void USkillWidget::SetSkillETime(const float RemainTime)
{
	if(SkillSlotE == nullptr)	return;
	SkillSlotE->SetSkillCoolTimeText(RemainTime);
}