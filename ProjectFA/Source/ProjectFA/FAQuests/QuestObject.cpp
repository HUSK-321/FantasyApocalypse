// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestObject.h"

UQuestObject::UQuestObject()
	:
	bQuestCompleted(false)
{
}

void UQuestObject::SetQuestComplete()
{
	bQuestCompleted = true;
}

bool UQuestObject::IsQuestCompleted() const
{
	return bQuestCompleted;
}