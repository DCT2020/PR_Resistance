// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "PR_Resistance/StatesSystem/Status.h"
#include "Fire.generated.h"
/**
 * 
 */
class UStaticMeshComponent;
UCLASS()
class PR_RESISTANCE_API UFire : public UCState
{
	GENERATED_BODY()
protected:
	FStatus* mCharacterStatus = nullptr;
	UWorld* mWorld = nullptr;

	USkeletalMeshComponent* mStaticMeshComp = nullptr;
	FTransform* mCharacterTransform = nullptr;
	UClass* mProjectileClass = nullptr;
	
	float mElapsedTime = 0.0f;


	bool _Init() override;

public:
	UFire();
	~UFire();

	bool Begin(CharacterState prevState) override;
	void Update(float deltaTime) override;
	void End() override;

};
