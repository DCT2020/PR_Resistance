// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboManager.h"

ComboManager::ComboManager()
{
}

ComboManager::~ComboManager()
{
}

void ComboManager::Init(UDataTable* actionTable, UAnimInstance* animInstance)
{
	mOwnerAnimInst = animInstance;

	TArray<FAction> rows;
	for (auto it : actionTable->GetRowMap())
	{
		FAction* action = reinterpret_cast<FAction*>(it.Value);
		Actions.Add(it.Key, action);
		if (action->bIsStartAttack)
		{
			StartActions.Add(action->InputBinding,action);
		}
	}
}

void ComboManager::Update(float deltaTime)
{
	if (mbIsWait)
	{
		if (mEalsedTime <= mCurWaitTime)
		{
			mEalsedTime += deltaTime;
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("%f : elapsedTime"), mEalsedTime));

			if (!mOwnerAnimInst->IsPlayingSlotAnimation(mCurAction->Animation, TEXT("DefaultSlot")))
			{
				SetComboEnd();
			}
		}
		else
		{
			SetComboEnd();
		}
	}
}

bool ComboManager::StartAttack(FName firstAttack)
{
	ChangeAction(firstAttack);

	return false;
}

void ComboManager::StartWaitInput()
{
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Blue,TEXT("Notified"));
	mbIsWait = true;
	mEalsedTime = 0.0f;
}

void ComboManager::PushInput(ActionInput inputType)
{
	if (mCurAction == nullptr)
	{
		auto action = StartActions[inputType];
		if (action != nullptr)
		{
			ChangeAction(action);
		}
	}
	else if (mbIsWait)
	{
		for (auto it : mCurAction->NextActions)
		{
			const FAction* temp = Actions[it];
			if (temp != nullptr)
			{
				if (temp->InputBinding == inputType)
				{
					ChangeAction(temp);
					return;
				}
			}
		}
	}
}

void ComboManager::BindComboAndEvent(std::function<void()> function)
{
	ComboEndEvent = function;
}

bool ComboManager::ChangeAction(FName actionName)
{
	auto action = Actions[actionName];
	return ChangeAction(action);
}

bool ComboManager::ChangeAction(const FAction* action)
{
	if (action != nullptr)
	{
		mEalsedTime = 0.0f;
		mbIsWait = false;
		mCurAction = action;
		mCurWaitTime = mCurAction->ComboWaitTime;
		mOwnerAnimInst->StopSlotAnimation(0.25f, TEXT("DefaultSlot"));
		mOwnerAnimInst->PlaySlotAnimationAsDynamicMontage(mCurAction->Animation, TEXT("DefaultSlot"),0.0f);
		return true;
	}

	return false;
}

void ComboManager::SetComboEnd()
{
	mEalsedTime = 0.0f;
	mbIsWait = false;
	mCurAction = nullptr;
	mOwnerAnimInst->StopSlotAnimation(0.0f,TEXT("DefaultSlot"));

	ComboEndEvent();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("ComboEnd"));
}
