// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Status.h"
#include "StateDesc.h"
#include "IState.generated.h"
/**
 * 
 */
class CharacterDataArchive;
 UINTERFACE(Blueprintable)
class PR_RESISTANCE_API UState : public UInterface
{
	GENERATED_BODY()
};

 class PR_RESISTANCE_API IState
 {
	 GENERATED_BODY()
 public:
	 virtual bool Init(CharacterDataArchive* archive) = 0;
	 virtual bool Begin(CharacterState prevState) = 0;
	 virtual void Update(float deltaTime) = 0;
	 virtual void End() = 0;
	 virtual void SetStart() = 0;
	 virtual void SetStop() = 0;
	 

	 virtual FStateDesc GetStateDesc() = 0;
 };