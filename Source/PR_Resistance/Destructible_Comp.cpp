// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible_Comp.h"

// Sets default values for this component's properties
UDestructible_Comp::UDestructible_Comp(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UDestructible_Comp::Init(UStaticMeshComponent* targetStateMesh, UFloatsComponent* floatcomp, uint8 indexOfHpInFloatsComp)
{
	mOwnerStaticMesh = targetStateMesh;
	floatcomp->AddListener(this, indexOfHpInFloatsComp);

	bIsInitCalled = true;
}

// Called when the game starts
void UDestructible_Comp::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsInitCalled)
	{
		UE_LOG(PRR, Error, TEXT("please call %s::Init(...) function"), *this->GetClass()->GetName());
	}
}


// Called every frame
void UDestructible_Comp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// IFloatListener
void UDestructible_Comp::ListenFloat(float newFloat)
{
}

void UDestructible_Comp::Init_bp(const UStaticMeshComponent* targetStateMesh, const UFloatsComponent* floatcomp, const uint8 indexOfHpInFloatsComp)
{
	Init(const_cast<UStaticMeshComponent*>(targetStateMesh),const_cast<UFloatsComponent*>(floatcomp),indexOfHpInFloatsComp);
}

