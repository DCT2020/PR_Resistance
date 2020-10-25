// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState_PlayerBase.h"
#include "PR_Resistance/StatesSystem/Status.h"
#include "Fire.generated.h"
/**
 * 
 */
struct FSoundData;
class UDataTable;
class UStaticMeshComponent;
class UFloatsComponent;
UCLASS()
class PR_RESISTANCE_API UFire : public UCState_PlayerBase
{
	GENERATED_BODY()
protected:
	FStatus* mCharacterStatus = nullptr;
	UWorld* mWorld = nullptr;

	USkeletalMeshComponent* mStaticMeshComp = nullptr;
	FTransform* mCharacterTransform = nullptr;
	UClass* mProjectileClass = nullptr;
	
	UDataTable* mSoundTable = nullptr;
	FSoundData* mSoundCue = nullptr;
	AActor* mOwner = nullptr;

	USkeletalMeshComponent* mRifleMesh = nullptr;
	UParticleSystem* mFireEffect = nullptr;
	float mElapsedTime = 0.0f;

	UFloatsComponent* mFloats;

	bool _Init() override;

public:
	UFire();
	~UFire();

	bool Begin(uint8 prevState) override;
	void Update(float deltaTime) override;
	void End() override;

};
