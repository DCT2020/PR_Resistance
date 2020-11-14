// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatsComponent.h"

// Sets default values for this component's properties
UFloatsComponent::UFloatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


UFloatsComponent::~UFloatsComponent()
{
	for (int i = 0; i < mListeners.Num(); ++i)
	{
		delete mListeners[i];
	}

	mListeners.Empty();
}

// Called when the game starts
void UFloatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UFloatsComponent::MakeFloats(uint8 size)
{
	if (mFloats.Num() < size)
	{
		mFloats.SetNum(size);
		for (int i = mListeners.Num(); i < size; ++i)
		{
			mListeners.Emplace(new TArray<IFloatListener*>);
			if (!mConditionCheckers.IsValidIndex(i))
			{
				mConditionCheckers.Emplace(TArray<FDele_CheckCondition>());
			}
		}
	}
}

bool UFloatsComponent::AddListener(IFloatListener* newFloatListener, uint8 index)
{
	if (!mListeners.IsValidIndex(index))
	{
		return false;
	}
	else
	{
		auto listener = mListeners[index];
		listener->Add(newFloatListener);
	}

	return true;
}

void UFloatsComponent::AddConditionChecker(FDele_CheckCondition func, uint8 index)
{
	if (!mConditionCheckers.IsValidIndex(index))
	{
		mConditionCheckers.EmplaceAt(index,TArray<FDele_CheckCondition>());
	}

	mConditionCheckers[index].Add(func);
}

int UFloatsComponent::PushBack(float newValue)
{
	mFloats.Add(newValue);
	mOriginFloats.Add(newValue);
	mListeners.Emplace(new TArray<IFloatListener*>);
	if (!mConditionCheckers.IsValidIndex(mFloats.Num() - 1))
	{
		mConditionCheckers.Emplace(TArray<FDele_CheckCondition>());
	}
	return mFloats.Num();
}

bool UFloatsComponent::Set(const float newValue, uint8 index)
{
	if(!mFloats.IsValidIndex(index))
		return false;

	for (auto conditionChecker : mConditionCheckers[index])
	{
		conditionChecker.Execute(index, mOriginFloats[index],newValue, mFloats[index]);
	}

	mFloats[index] = newValue;
	
	for (auto listener : *mListeners[index])
	{
		listener->ListenFloat(index, mFloats[index]);
	}

	return true;
}

bool UFloatsComponent::Get(uint8 index, float& out)
{
	if (!mFloats.IsValidIndex(index))
		return false;

	out = mFloats[index];
	return true;
}

int UFloatsComponent::GetSize()
{
	return mFloats.Num();
}


void UFloatsComponent::MakeFloats_bp(uint8 size)
{
	MakeFloats(size);
}
// Blueprint call
void UFloatsComponent::PushBack_bp(float newValue, int& size)
{
	size = PushBack(newValue);
}

void UFloatsComponent::Get_bp(uint8 index, float& value, bool& isValid)
{
	isValid = Get(index,value);
}

void UFloatsComponent::Set_bp(float newValue, uint8 index, bool& isValid)
{
	isValid = Set(newValue,index);
}

void UFloatsComponent::AddConditionChecker_bp(uint8 index, const FDele_CheckCondition& func)
{
	AddConditionChecker(func, index);
}

void UFloatsComponent::AddListener_bp(uint8 index, const TScriptInterface<IFloatListener>& newFloatListener)
{
	AddListener(static_cast<IFloatListener*>(newFloatListener.GetInterface()), index);
}
