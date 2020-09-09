// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IState.h"
/**
 * 
 */
class PR_RESISTANCE_API Walk : public IState
{
public:
	Walk(FStatus const* status);
	virtual ~Walk();

	virtual bool Init() override;
	virtual bool Begin() override;
	virtual void Update(float deltaTime) override;
	virtual void End() override;
	virtual int GetPriority() override;
};
