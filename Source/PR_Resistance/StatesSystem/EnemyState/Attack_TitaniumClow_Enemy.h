// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "Attack_TitaniumClow_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PR_RESISTANCE_API UAttack_TitaniumClow_Enemy : public UCState
{
	GENERATED_BODY()


public:
	virtual bool Begin(uint8 prevState) override;
	virtual void Update(float deltaTime) override;
	virtual void End() override;
protected:
	virtual bool _Init() override;
};

