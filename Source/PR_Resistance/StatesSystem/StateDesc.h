// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateDesc.generated.h"

UENUM(BlueprintType)
enum class CharacterState : uint8
{
	CS_NULL = 0,
	CS_IDLE UMETA(DisplayName = "Idle"),
	CS_WALK UMETA(DisplayName = "Walk")
};

/**
 * 
 */
 USTRUCT(Atomic, BlueprintType)
struct FStateDesc
{
	GENERATED_BODY()
	
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
	CharacterState StateType = CharacterState::CS_NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Priority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsManualStop = false;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<UState> State;*/
};
