// Fill out your copyright notice in the Description page of Project Settings.


#include "FAGameInstance.h"
#include "FAQuests/PlayerQuestManagement.h"

UFAGameInstance::UFAGameInstance()
{
}

void UFAGameInstance::Init()
{
	Super::Init();
	
	if(PlayerQuestManagementClass)
	{
		PlayerQuestManagement = NewObject<UPlayerQuestManagement>(this, PlayerQuestManagementClass);
		if(PlayerQuestManagement && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("PlayerQuestManagement 만듬")));
		}
	}
}