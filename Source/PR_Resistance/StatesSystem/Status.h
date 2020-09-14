// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Status.generated.h"

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct FStatus
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere)
		float maxHP;
	UPROPERTY(EditAnywhere)
		float curHP;
	UPROPERTY(EditAnywhere)
		float maxStamina;
	UPROPERTY(EditAnywhere)
		float curStamina;
	UPROPERTY(EditAnywhere)
		float walkSpeed;
	UPROPERTY(EditAnywhere)
		float runSpeed;
	UPROPERTY(EditAnywhere)
		float DodgeStamina;
	UPROPERTY(EditAnywhere)
		float DodgeTime;
	UPROPERTY(EditAnywhere)
		float DodgeDistance;
	UPROPERTY(EditAnywhere)
		float JumpDashStamina;
	UPROPERTY(EditAnywhere)
		float JumpDashTime;
	UPROPERTY(EditAnywhere)
		float JumpDashDistance;
};

		