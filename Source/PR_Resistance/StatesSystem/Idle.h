// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IState.h"
/**
 *	use in PR_ResistanceCharacter 
 * 
 */
class PR_RESISTANCE_API Idle : public IState
{
public:
	Idle(FStatus const* status);
	virtual ~Idle();

	virtual bool Init()		override;
	virtual bool Begin()	override;
	virtual void Update(float deltaTime)	override;
	virtual void End()		override;
	virtual int GetPriority() override;

};
