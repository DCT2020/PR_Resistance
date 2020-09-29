// Fill out your copyright notice in the Description page of Project Settings.


#include "UAttack.h"
#include "Animation/AnimInstance.h"
#include "Engine/DataTable.h"

#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Combo/ComboManager.h"

#include <functional>

UAttack::UAttack()
{
	mDesc.StateType = CharacterState::CS_ATTACK;
	mDesc.Priority = 3;
}

UAttack::~UAttack()
{
	if(mComboManager != nullptr)
	{
		delete mComboManager;
		mComboManager = nullptr;
	}
}

bool UAttack::Begin(CharacterState prevState)
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
	void* temp = nullptr;
	bool result = false;
	UAnimInstance* mAnimInstance = nullptr;
	UDataTable* mActionTable = nullptr;
	std::function<void()>* notifer;

	result = GetCharacterDataArchive()->GetData("AnimInstance", &temp);
	if (result)
	{
		mAnimInstance = (UAnimInstance*)temp;
	}
	result = GetCharacterDataArchive()->GetData("ActionTable", &temp);
	if (result)
	{
		mActionTable = (UDataTable*)temp;
	}
	result = GetCharacterDataArchive()->GetData("AInput_Change", &temp);
	if (result)
	{
		AInput = (ActionInput*)temp;
	}
	result = GetCharacterDataArchive()->GetData("TimeToNextStepNotify", &temp);
	if (result)
	{
		notifer = (std::function<void()>*)temp;
		(*notifer) = [this]() {mComboManager->StartWaitInput(); };
	}

	if (mComboManager == nullptr)
	{
		mComboManager = new ComboManager();
		mComboManager->Init(mActionTable, mAnimInstance);
		mComboManager->BindComboAndEvent([this]() {StopAttack(); });
	}

	return true;
}

void UAttack::StopAttack()
{
	mDesc.bIsEnd = true;
}
