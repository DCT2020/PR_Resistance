// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/IState.h"
/**
 *	use in PR_ResistanceCharacter 
 * 
 */
class PR_RESISTANCE_API UIdle : public IState
{
	FStateDesc temp;
public:
	UIdle();
	virtual ~UIdle();

	virtual bool		Init()					override;
	virtual bool		Begin()					override;
	virtual void		Update(float deltaTime)	override;
	virtual void		End()					override;
	virtual FStateDesc	GetDesc()				override;

};
