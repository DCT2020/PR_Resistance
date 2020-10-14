// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataArchive.h"

UCharacterDataArchive::UCharacterDataArchive()
{
}

UCharacterDataArchive::~UCharacterDataArchive()
{
}

void UCharacterDataArchive::Init()
{
	mDatas.Empty();
}

bool UCharacterDataArchive::AddData(FName key, void* data)
{
	void* result = mDatas.Add(key,data);
	if (result == nullptr)
	{
		return false;
	}

	return true;
}

void UCharacterDataArchive::RemoveData(FName key)
{
	mDatas.Remove(key);
}

bool UCharacterDataArchive::GetData(FName key, _Out_ void** data)
{
	*data = nullptr;
	if (mDatas.Contains(key))
	{
		*data = mDatas[key];
		return true;
	}

	return false;
}
