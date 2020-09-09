// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Status.h"
/**
 * 
 */
class PR_RESISTANCE_API IState
{
public:
	IState(FStatus const * status){}
	virtual ~IState(){}

public:
	virtual bool Init() = 0;
	virtual bool Begin() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void End() = 0;

	virtual int GetPriority() = 0;
};

