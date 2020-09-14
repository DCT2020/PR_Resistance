// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
/**
 *	use in PR_ResistanceCharacter 
 * 
 */
class PR_RESISTANCE_API UIdle : public CState
{
public:
	UIdle();
	virtual ~UIdle();

	virtual bool		_Init()					override;
	virtual bool		Begin(CharacterState prevState)					override;
	virtual void		Update(float deltaTime)	override;
	virtual void		End()					override;
};
