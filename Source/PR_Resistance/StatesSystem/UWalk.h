// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
/**
 * 
 */
class PR_RESISTANCE_API UWalk : public CState
{
public:
	FStatus* mCharacterStatus = nullptr;
	float* mMaxWalkSpeed = nullptr;

public:
	UWalk();
	virtual ~UWalk();

	virtual bool		_Init()					override;
	virtual bool		Begin(CharacterState prevState)					override;
	virtual void		Update(float deltaTime) override;
	virtual void		End()					override;
};
