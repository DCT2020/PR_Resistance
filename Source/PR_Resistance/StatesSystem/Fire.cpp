// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Object/Projectile.h"
#include "Kismet/GameplayStatics.h"
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
	GetCharaDataWithLog("World", &buffer);
	{
		mWorld = (UWorld*)(buffer);
	}
	GetCharaDataWithLog("Status", &buffer);
	{
		mCharacterStatus = (FStatus*)(buffer);
	}
	GetCharaDataWithLog("CharacterTransform", &buffer);
	{
		mCharacterTransform = (FTransform*)(buffer);
	}
	GetCharaDataWithLog("SkeletalMeshComponent", &buffer);
	{
		mStaticMeshComp = (USkeletalMeshComponent*)(buffer);
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

				FVector2D screenSize;
				FVector direciton;
				FVector position;
				auto playerController = UGameplayStatics::GetPlayerController(mStaticMeshComp->GetOwner()->GetWorld(),0);
				GEngine->GameViewport->GetViewportSize(screenSize);
				screenSize *= 0.5f;
				UGameplayStatics::DeprojectScreenToWorld(playerController, screenSize, position, direciton);
				projectile->Init(direciton,mCharacterStatus->BulletDamage,mCharacterStatus->BulletSpeed,mCharacterStatus->BulletLifeTime);
			}
		}
	}
}

void Fire::End()
{
}
