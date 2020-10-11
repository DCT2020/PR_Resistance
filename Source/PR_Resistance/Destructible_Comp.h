// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Destructible_Comp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PR_RESISTANCE_API UDestructible_Comp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDestructible_Comp();

	UStaticMeshComponent* mOwnerStaticMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Destructible)
	UStaticMesh* 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
