// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

//my class
#include "PR_Resistance/Component/FloatsComponent.h"
#include "PR_Resistance/StatesSystem/Managers/StateManager.h"

#include "Enemy.generated.h"

UCLASS()
class PR_RESISTANCE_API AEnemy : public APawn , public IFloatListener
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

	UPROPERTY()
		UFloatsComponent* mFloatsComponent = nullptr;
	
	UPROPERTY()
		UStateManager* mStateManager = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ListenFloat(int index, float newFloat) override;
};
