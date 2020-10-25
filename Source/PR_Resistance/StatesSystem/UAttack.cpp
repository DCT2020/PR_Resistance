// Fill out your copyright notice in the Description page of Project Settings.


#include "UAttack.h"
#include "Animation/AnimInstance.h"
#include "Engine/DataTable.h"

#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Combo/ComboManager.h"

#include <functional>

UAttack::UAttack()
{
	mDesc.StateType = (uint8)CharacterState::CS_ATTACK;
	mDesc.Priority = 3;
}

UAttack::~UAttack()
{
}

bool UAttack::Begin(uint8 prevState)
{
	return true;
}

void UAttack::Update(float deltaTime)
{
	mComboManager->Update(deltaTime);
	if ((*AInput) != ActionInput::AINPUT_NULL)
	{
		mComboManager->PushInput((*AInput));
		(*AInput) = ActionInput::AINPUT_NULL;
	}
}

void UAttack::End()
{
}

void UAttack::SetProvider(IStaminaProvider* provider)
{
	mSPProvider = provider;
}

void UAttack::SetStop()
{
	mComboManager->SetComboEnd();
}

bool UAttack::_Init()
{
	void* buffer = nullptr;
	UAnimInstance* mAnimInstance = nullptr;
	UDataTable* mActionTable = nullptr;
	std::function<void()>* notifer;

	GetCharaDataWithLog("AnimInstance", &buffer);
	{
		mAnimInstance = (UAnimInstance*)buffer;
	}
	GetCharaDataWithLog("ActionTable", &buffer);
	{
		mActionTable = (UDataTable*)buffer;
	}
	GetCharaDataWithLog("AInput_Change", &buffer);
	{
		AInput = (ActionInput*)buffer;
	}
	GetCharaDataWithLog("TimeToNextStepNotify", &buffer);
	{
		notifer = (std::function<void()>*)buffer;
		(*notifer) = [this]() {mComboManager->StartWaitInput(); };
	}

	if (mComboManager == nullptr)
	{
		  GetCharaDataWithLog("Owner",&buffer)
		mComboManager = NewObject<UComboManager>();
		mComboManager->Init(static_cast<APR_ResistanceCharacter*>(buffer),mActionTable, mAnimInstance);
		mComboManager->BindComboAndEvent([this]() {StopAttack(); });
	}

	return true;
}

void UAttack::StopAttack()
{
	mDesc.bIsEnd = true;
}
