// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TimeToNextStepNotify.generated.h"

/**
 * 
 */
UCLASS()
class PR_RESISTANCE_API UTimeToNextStepNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	
private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
