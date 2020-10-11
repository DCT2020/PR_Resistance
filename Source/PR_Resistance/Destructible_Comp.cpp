// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible_Comp.h"

// Sets default values for this component's properties
UDestructible_Comp::UDestructible_Comp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AActor* owner = GetOwner();
	mOwnerStaticMesh = Cast<UStaticMeshComponent>(owner->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (mOwnerStaticMesh == nullptr)
	{
		
	}


	// ...
}


// Called when the game starts
void UDestructible_Comp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDestructible_Comp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

