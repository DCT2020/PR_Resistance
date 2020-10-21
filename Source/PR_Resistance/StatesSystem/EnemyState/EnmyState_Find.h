// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/EnemyState/EnmyStateBase.h"
#include "EnmyState_Find.generated.h"

/**
 * 
 */
UCLASS()
class PR_RESISTANCE_API UEnmyState_Find : public UEnmyStateBase
{
	GENERATED_BODY()
protected:


   // ------------------ Blueprintable
        UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Detecting)
	TSet<AActor*> mDetectedTargets;

        
public:
	UEnmyState_Find();
	
	virtual bool Begin(uint8 prevState) override;
	virtual void Update(float deltaTime) override;
	virtual void End() override;
protected:
	virtual bool _Init() override;

protected:
	UFUNCTION()
		void OnDetect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
                void OnOutDetect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public: /////// Blueprint callable functions
};
