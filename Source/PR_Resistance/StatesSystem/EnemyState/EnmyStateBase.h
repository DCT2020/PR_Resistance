// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"

#include "Components/SphereComponent.h"

#include "EnmyStateBase.generated.h"

UENUM()
enum class EEnemyState : uint8
{
	ES_IDLE UMETA(DisplayName = "Idle"),
	ES_FIND UMETA(DisplayNAme = "FindEnemy"),
	ES_TRYATTACK UMETA(DisplayName = "TryAttack"),


	ES_TITANIUMCLOW UMETA(DisplayName = "TitaniumClow"),
	ES_CHARGING_IMPACT UMETA(DisplayName = "ChargingEffect")
};

USTRUCT()
struct FEnemyStateData
{
	GENERATED_BODY()

	UPROPERTY()
        USphereComponent* mDetectingCollider;

        UPROPERTY()
        AActor* mOwner;
};

/**
 * 
 */
UCLASS()
class PR_RESISTANCE_API UEnmyStateBase : public UCState
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		FEnemyStateData mData;

	virtual void InitStateData(const FEnemyStateData& data)
	{
		mData = data;
	}
	
};
