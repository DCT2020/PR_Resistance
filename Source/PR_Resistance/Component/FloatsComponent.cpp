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

int UFloatsComponent::PushBack(float newValue)
{
	mFloats.Add(newValue);
	mListeners.Emplace(new TArray<IFloatListener*>);
	return mFloats.Num();
}

bool UFloatsComponent::Set(const float newValue, uint8 index)
{
	if(!mFloats.IsValidIndex(index))
		return false;

	mFloats[index] = newValue;
	for (auto listener : *mListeners[index])
	{
		listener->ListenFloat(mFloats[index]);
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
