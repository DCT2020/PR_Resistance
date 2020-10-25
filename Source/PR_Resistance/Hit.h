// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatesSystem/CState.h"
#include "Hit.generated.h"

/**
 * 
 */
struct FCharacterAnimationData;
class UDataTable;
class APR_ResistanceCharacter;
UCLASS()
class PR_RESISTANCE_API UHit : public UCState
{
	GENERATED_BODY()
public:
	UPROPERTY()
		UAnimInstance* mAnimInstace = nullptr;
	UPROPERTY()
		APR_ResistanceCharacter* mOwner;

	UPROPERTY()
		UAnimMontage* mMontage;


	UDataTable* mAnimTable = nullptr;
	FCharacterAnimationData* mHitAnimation;
public:
	UHit();
        virtual ~UHit();
	
	virtual bool Begin(uint8 prevState) override;
	virtual void Update(float deltaTime) override;
	virtual void End() override;

	UFUNCTION()
	void OnHitEnd(UAnimMontage* Montage, bool bInterrupted);
protected:
	virtual bool _Init() override;
};
