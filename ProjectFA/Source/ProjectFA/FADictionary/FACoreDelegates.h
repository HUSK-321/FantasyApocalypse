// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyDestroyed, UObject*);

class PROJECTFA_API FACoreDelegates
{
public:
	FACoreDelegates();
	~FACoreDelegates();

	static FOnEnemyDestroyed OnEnemyDestroyed;
};