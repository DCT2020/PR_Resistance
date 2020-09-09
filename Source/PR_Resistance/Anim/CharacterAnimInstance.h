// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PR_RESISTANCE_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
    UCharacterAnimInstance();

private:
    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Pawn,
    Meta = (AllowPrivateAccess=true));
    float CurrentPawnSpeed;
};
