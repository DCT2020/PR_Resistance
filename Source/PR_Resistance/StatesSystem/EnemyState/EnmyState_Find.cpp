// Fill out your copyright notice in the Description page of Project Settings.


#include "EnmyState_Find.h"

UEnmyState_Find::UEnmyState_Find()
{
	mDesc.Priority = 1;
	mDesc.StateType = (uint8)EEnemyState::ES_FIND;
}

bool UEnmyState_Find::Begin(uint8 prevState)
{
	mData.mDetectingCollider->OnComponentBeginOverlap.AddDynamic(this, &UEnmyState_Find::OnDetect);
	mData.mDetectingCollider->OnComponentEndOverlap.AddDynamic(this, &UEnmyState_Find::OnOutDetect);

	return true;
}

void UEnmyState_Find::Update(float deltaTime)
{
	
}

void UEnmyState_Find::End()
{
}

bool UEnmyState_Find::_Init()
{
	return true;
}

void UEnmyState_Find::OnDetect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	mDetectedTargets.Add(OtherActor);
}

void UEnmyState_Find::OnOutDetect(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	mDetectedTargets.Remove(OtherActor);
}
