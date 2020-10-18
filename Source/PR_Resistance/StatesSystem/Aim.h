// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "Aim.generated.h"

/**
 * 
 */
UCLASS()
class PR_RESISTANCE_API UAim : public UCState
{
	GENERATED_BODY()
	
public:
	UAim();

	bool Begin(uint8 prevState) override;
	void Update(float deltaTime) override;
	void End() override;

protected:
	bool _Init() override;

};
