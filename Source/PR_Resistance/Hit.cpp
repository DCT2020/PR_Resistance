// Fill out your copyright notice in the Description page of Project Settings.


#include "Hit.h"
#include "Engine/DataTable.h"
#include "PR_Resistance/Chara/PR_ResistanceCharacter.h"

UHit::UHit()
{
	mDesc.StateType = (uint8)CharacterState::CS_HIT;
	mDesc.Priority = 10;
}

bool UHit::Begin(uint8 prevState)
{
	mOwner->StopSlotAnimation_onServrer(TEXT("DefaultSlot"));
	mOwner->StopSlotAnimation_onServrer(TEXT("UpperMotion"));
	mOwner->PlaySlotAnimation_onServrer(TEXT("ParalleMotion"), mHitAnimation->mAnimation);
	
	return true;
}

void UHit::Update(float deltaTime)
{
}

void UHit::End()
{
}

bool UHit::_Init()
{
	void* buffer;
	GetCharaDataWithLog("AnimInstance", &buffer);
	{
		mAnimInstace = (UAnimInstance*)buffer;
	}
	GetCharaDataWithLog("AnimTable", &buffer);
	{
		mAnimTable = (UDataTable*)buffer;
	}
	GetCharaDataWithLog("Owner", &buffer);
	{
		mOwner = (APR_ResistanceCharacter*)buffer;
	}
	
	mHitAnimation = mAnimTable->FindRow<FCharacterAnimationData>(TEXT("Hit"),nullptr);
	
	return true;
}