// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Object/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "PR_Resistance/Chara/PR_ResistanceCharacter.h"
#include "Components/StaticMeshComponent.h"

UFire::UFire()
{
	mDesc.StateType = (uint8)CharacterState::CS_ATTACK;
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
	GetCharaDataWithLog("SoundTable", &buffer);
	{
		mSoundTable = (UDataTable*)buffer;
	}
	GetCharaDataWithLog("Owner", &buffer)
	{
		mOwner = (AActor*)buffer;
	}
	GetCharaDataWithLog("RifleMesh",&buffer)
	{
		mRifleMesh = (USkeletalMeshComponent*)buffer;
	}
	GetCharaDataWithLog("FireEffect", &buffer)
	{
		mFireEffect = (UParticleSystem*)buffer;
	}
	GetCharaDataWithLog("Floats", &buffer)
        {
		mFloats = (UFloatsComponent*)buffer;
	}
	
	return isInited;
}

bool UFire::Begin(uint8 prevState)
{
	if(prevState == (uint8)CharacterState::CS_RUN)
		return false;

	if (mCurrentStateInfos->mCurSubState != CharacterState::CS_SUB_AIM)
		return false;

	mSoundCue =  mSoundTable->FindRow<FSoundData>(TEXT("SMG"), nullptr);

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

		float out = 0.0f;
		mFloats->Get((uint8)EPlayerFloats::PF_AMMO, out);
		mFloats->Set(out - 1.0f, (uint8)EPlayerFloats::PF_AMMO);
		if (out <= 0.0f)
		{
			return;
		}

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

				UWorld* world = mOwner->GetWorld();

				
				FTransform FireTransform = mRifleMesh->GetSocketTransform(TEXT("FirePoint"));

				world->SpawnActor<AActor>(mSoundCue->mSoundActorClass, FireTransform);
				//
				//UGameplayStatics::PlaySoundAtLocation(mOwner, mSoundCue->mSound,
				//	mStaticMeshComp->GetSocketLocation(TEXT("UFirePoint")),
				//	mStaticMeshComp->GetSocketRotation(TEXT("UFirePoint")));
				//
				//UGameplayStatics::SpawnEmitterAtLocation(mOwner, mFireEffect,
				//	mRifleMesh->GetSocketLocation(TEXT("FirePoint")),
				//	mRifleMesh->GetSocketRotation(TEXT("FirePoint")));
			}
		}
	}
}

void UFire::End()
{
}
