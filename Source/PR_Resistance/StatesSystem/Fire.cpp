// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Object/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

UFire::UFire()
{
	mDesc.StateType = CharacterState::CS_ATTACK;
	mDesc.Priority = 3;
}

UFire::~UFire()
{
	
}

bool UFire::_Init()
{
	bool isInited = UCState_PlayerBase::_Init();  

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

	return isInited;
}

bool UFire::Begin(CharacterState prevState)
{
	if(prevState == CharacterState::CS_RUN)
		return false;

	if (mCurrentStateInfos->mCurSubState != CharacterState::CS_SUB_AIM)
		return false;


	return true;
}

void UFire::Update(float deltaTime)
{
	if (mCharacterStatus->FireDelayTime >= mElapsedTime)
	{
		mElapsedTime += deltaTime;
	}
	else
	{
		mElapsedTime = 0.0f;

		FTransform tempTransform = mStaticMeshComp->GetSocketTransform("UFirePoint");
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

void UFire::End()
{
}
