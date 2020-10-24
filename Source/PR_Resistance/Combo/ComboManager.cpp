// Fill out your copyright notice in the Description page of Project Settings.

#include "ComboManager.h"
#include "PR_Resistance/Chara/PR_ResistanceCharacter.h"

UComboManager::UComboManager()
{
}

UComboManager::~UComboManager()
{
}

void UComboManager::Init(TQueue<FSlotMotionProcess>* _SlotMotionQueue, UDataTable* actionTable, UAnimInstance* animInstance)
{
	mSlotMotionQueue = _SlotMotionQueue;
	mOwnerAnimInst = animInstance;

	TArray<FAction> rows;
	for (auto it : actionTable->GetRowMap())
	{
		FAction* action = reinterpret_cast<FAction*>(it.Value);
		Actions.Add(it.Key, action);
		if (action->bIsStartAttack)
		{
			StartActions.Add(action->InputBinding, action);
		}
	}

	mCurAction = nullptr;
}

void UComboManager::Update(float deltaTime)
{
	if (mbIsWait)
	{
		if (mEalsedTime <= mCurWaitTime)
		{
			mEalsedTime += deltaTime;
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("%f : elapsedTime"), mEalsedTime));

			//if (!mOwnerAnimInst->IsPlayingSlotAnimation(mCurAction->Animation, TEXT("DefaultSlot")))
			//{
			//	SetComboEnd();
			//}
		}
		else
		{
			SetComboEnd();
		}
	}
}

bool UComboManager::StartAttack(FName firstAttack)
{
	ChangeAction(firstAttack);

	return false;
}

void UComboManager::StartWaitInput()
{
	mOwnerAnimInst->Montage_Pause(mCurDynmMontage);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Notified"));
	mbIsWait = true;
	mEalsedTime = 0.0f;
}

void UComboManager::PushInput(ActionInput inputType)
{
	if (mCurAction == nullptr)
	{
		if (StartActions.Num() == 0)
			return;
		
		auto action = StartActions[inputType];
		if (action != nullptr)
		{
			ChangeAction(action);
		}
	}
	else if (mbIsWait)
	{
		if (Actions.Num() == 0)
			return;
		
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

void UComboManager::BindComboAndEvent(std::function<void()> function)
{
	ComboEndEvent = function;
}


bool UComboManager::ChangeAction(FName actionName)
{
	auto action = Actions[actionName];
	return ChangeAction(action);
}

bool UComboManager::ChangeAction(const FAction* action)
{
	if (action != nullptr)
	{
		mEalsedTime = 0.0f;
		mbIsWait = false;
		mCurAction = action;
		mCurWaitTime = mCurAction->ComboWaitTime;
		
		StopSlotAnimation(TEXT("DefaultSlot"));
		//mOwnerAnimInst->PlaySlotAnimationAsDynamicMontage(mCurAction->Animation, TEXT("DefaultSlot"),0.0f);
		//mCurDynmMontage = mOwnerAnimInst->PlaySlotAnimationAsDynamicMontage(mCurAction->Animation, TEXT("DefaultSlot"),0.0f,0.0f);
		PlayerSlotAnimation(TEXT("DefaultSlot"), mCurAction->Animation);
		return true;
	}

	return false;
}

void UComboManager::PlayerSlotAnimation(FName slotName, UAnimSequenceBase *animSequence)
{
	FSlotMotionProcess process;
	process.mLamda = [this, slotName, animSequence]()
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("PlaySlotAnimation"));

		if (mOwnerAnimInst->TryGetPawnOwner()->GetLocalRole() == ROLE_Authority)
		{
			mCurDynmMontage = mOwnerAnimInst->PlaySlotAnimationAsDynamicMontage(animSequence, slotName, 0.0f, 0.0f);
		}
		else
		{
			mOwnerAnimInst->PlaySlotAnimationAsDynamicMontage(animSequence, slotName, 0.0f, 0.0f);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%s, %s"), *animSequence->GetPathName(), *slotName.ToString()));
		}

	};

	mSlotMotionQueue->Enqueue(process);
}

void UComboManager::StopSlotAnimation(FName slotName)
{
	FSlotMotionProcess process;
	process.mLamda = [this,slotName]()
	{
		mOwnerAnimInst->StopSlotAnimation(0.25f, slotName);
	};

	mSlotMotionQueue->Enqueue(process);
}

void UComboManager::SetComboEnd()
{
	mEalsedTime = 0.0f;
	mbIsWait = false;
	mCurAction = nullptr;
	mOwnerAnimInst->Montage_Stop(0.15f, mCurDynmMontage);
	mOwnerAnimInst->StopSlotAnimation(0.0f, TEXT("DefaultSlot"));

	if (ComboEndEvent != nullptr)
		ComboEndEvent();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("ComboEnd"));
}
