// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SearchOutEffectable.generated.h"

UINTERFACE(MinimalAPI)
class USearchOutEffectable : public UInterface
{
	GENERATED_BODY()
};


class PROJECTFA_API ISearchOutEffectable
{
	GENERATED_BODY()

public:
	/*
	 * @brief Enable custom depth for customdepth stencil value 100
	 */
	virtual void EnableSearchOutEffect() {}

	/*
	* @brief Disable custom depth for customdepth stencil value 100
	*/
	virtual void DisableSearchOutEffect() {}
};
