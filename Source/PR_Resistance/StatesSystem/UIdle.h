// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "UIdle.generated.h"
/**
 *	use in PR_ResistanceCharacter 
 * 
 */
 UCLASS()
class PR_RESISTANCE_API UIdle : public UCState
{
	GENERATED_BODY()
public:
	UIdle();
	virtual ~UIdle();

	virtual bool		_Init()					override;
	virtual bool		Begin(uint8 prevState)					override;
	virtual void		Update(float deltaTime)	override;
	virtual void		End()					override;
};
