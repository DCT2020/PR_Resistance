// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
 class IStaminaProvider;
class PR_RESISTANCE_API IStaminaUser
{
public:
	virtual void SetProvider(IStaminaProvider* provider) = 0;
};
