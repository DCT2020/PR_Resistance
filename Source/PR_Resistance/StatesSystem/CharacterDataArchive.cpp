// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataArchive.h"

CharacterDataArchive::CharacterDataArchive()
{
}

CharacterDataArchive::~CharacterDataArchive()
{
}

void CharacterDataArchive::Init()
{
	mDatas.Empty();
}

bool CharacterDataArchive::AddData(FName key, void* data)
{
	void* result = mDatas.Add(key,data);
	if (result == nullptr)
	{
		return false;
	}

	return true;
}

void CharacterDataArchive::RemoveData(FName key)
{
	mDatas.Remove(key);
}

bool CharacterDataArchive::GetData(FName key, _Out_ void** data)
{
	*data = nullptr;
	if (mDatas.Contains(key))
	{
		*data = mDatas[key];
		return true;
	}

	return false;
}
