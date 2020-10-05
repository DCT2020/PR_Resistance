// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "PR_Resistance/StatesSystem/Status.h"
/**
 * 
 */
 class UStaticMeshComponent;
class PR_RESISTANCE_API Fire : public CState
{
protected:
	FStatus* mCharacterStatus = nullptr;
	UWorld* mWorld = nullptr;

	USkeletalMeshComponent* mStaticMeshComp = nullptr;
	FTransform* mCharacterTransform = nullptr;
	UClass* mProjectileClass = nullptr;
	
	float mElapsedTime = 0.0f;


	bool _Init() override;

public:
	Fire();
	~Fire();

	bool Begin(CharacterState prevState) override;
	void Update(float deltaTime) override;
	void End() override;

};
