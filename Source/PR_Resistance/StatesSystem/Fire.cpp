// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Object/Projectile.h"
#include "Components/StaticMeshComponent.h"

Fire::Fire()
{
	mDesc.StateType = CharacterState::CS_ATTACK;
	mDesc.Priority = 3;
}

Fire::~Fire()
{
	
}

bool Fire::_Init()
{
	void* buffer = nullptr;
	auto result = GetCharacterDataArchive()->GetData(TEXT("World"), &buffer);
	if (result)
	{
		mWorld = (UWorld*)(buffer);
	}
	result = GetCharacterDataArchive()->GetData(TEXT("Status"), &buffer);
	if (result)
	{
		mCharacterStatus = (FStatus*)(buffer);
	}
	result = GetCharacterDataArchive()->GetData(TEXT("CharacterTransform"), &buffer);
	if (result)
	{
		mCharacterTransform = (FTransform*)(buffer);
	}
	result = GetCharacterDataArchive()->GetData(TEXT("StaticMeshComponenet"), &buffer);
	if (result)
	{
		mStaticMeshComp = (UStaticMeshComponent*)(buffer);
	}
	

	return true;
}

bool Fire::Begin(CharacterState prevState)
{
	return true;
}

void Fire::Update(float deltaTime)
{
	if (mCharacterStatus->FireDelayTime >= mElapsedTime)
	{
		mElapsedTime += deltaTime;
	}
	else
	{
		mElapsedTime = 0.0f;

		FTransform tempTransform = mStaticMeshComp->GetSocketTransform("FirePoint");
		auto actor = mWorld->SpawnActor(mCharacterStatus->ProjectileClass, &tempTransform);
		if (actor != nullptr)
		{
			auto projectile = Cast<AProjectile>(actor);
			if (projectile != nullptr)
			{
				FQuat rot = tempTransform.GetRotation();
				FVector lookAt = FVector::ForwardVector;
				lookAt = rot.RotateVector(lookAt);
				projectile->Init(lookAt,mCharacterStatus->DodgeStamina,mCharacterStatus->BulletSpeed,mCharacterStatus->BulletLifeTime);
			}
		}
	}
}

void Fire::End()
{
}
