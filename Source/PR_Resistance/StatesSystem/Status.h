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
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float maxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float curHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float curStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float walkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float runSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DodgeStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DodgeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DodgeDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpDashStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpDashTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpDashDistance;
	
	// TODO : 나중에 무기로 뺄 것
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireDelayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BulletSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BulletLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BulletDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* GunModelWith_FireSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxAmmo;
};

		