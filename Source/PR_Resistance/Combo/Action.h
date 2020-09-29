// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimSequence.h"
#include "Engine/DataTable.h"
#include "Action.generated.h"
/**
 * 
 */
 UENUM(BlueprintType)
enum class ActionInput : uint8
{
    AINPUT_NULL = 0,
    AINPUT_WEAKATTACK ,
    AINPUT_STRONGATTACK
};

USTRUCT(Atomic,BlueprintType)
struct PR_RESISTANCE_API FAction : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ActionInput InputBinding;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ComboWaitTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsStartAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLastAttack = false;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    UAnimSequence* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> NextActions;
};
