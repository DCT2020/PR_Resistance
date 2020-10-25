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
	CS_WALK UMETA(DisplayName = "Walk"),
	CS_RUN UMETA(DisplayName = "Run"),
	CS_DODGE UMETA(DisplayName = "Dodge"),
	CS_JUMP UMETA(DisplayName = "Jump"),
	CS_JUMPDASH UMETA(DisplayName = "JumpDash"),
	CS_ATTACK UMETA(DisplayName = "Attack"),
	CS_STRONGATTACK UMETA(DisplayName = "StrongAttack"),
	CS_AIM UMETA(DisplayName	 = "Aim"),
	CS_FIRE UMETA(DisplayName = "Fire"),
	CS_HIT UMETA(DisplayName = "Hit"),

	// sub state
	CS_SUB_IDLE UMETA(DisplayName = "Idle Substate"),
	CS_SUB_RELOAD UMETA(DisplayName = "Reload Substate"),
	CS_SUB_AIM UMETA(DisplayName = "Aim Substate"),
	CS_SUB_FIRE UMETA(DisplayName = "Fire Substate"),
};

/**
 * 
 */
 USTRUCT(Atomic, BlueprintType)
struct FStateDesc
{
	GENERATED_BODY()
	
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 StateType = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Priority = 0;

	bool bIsEnd = false;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<UState> State;*/
};
