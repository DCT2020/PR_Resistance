// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "Idle_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PR_RESISTANCE_API UIdle_Enemy : public UCState
{
	GENERATED_BODY()

public:
	virtual bool Begin(CharacterState prevState) override;
	virtual void Update(float deltaTime) override;
	
protected:
	virtual bool _Init() override;
};
