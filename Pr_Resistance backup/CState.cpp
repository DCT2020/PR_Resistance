// Fill out your copyright notice in the Description page of Project Settings.


#include "CState.h"

UCState::UCState()
{
}

UCState::~UCState()
{
}

FStateDesc UCState::GetStateDesc()
{
	return mDesc;
}

void UCState::SetStart()
{
	mDesc.bIsEnd = false;
}

void UCState::SetStop()
{
	mDesc.bIsEnd = true;
}

CharacterDataArchive * const UCState::GetCharacterDataArchive()
{
	return mDataArchive;
}

bool UCState::Init(CharacterDataArchive* archive)
{	
	if (archive == nullptr)
	{
		return false;
	}

	mDataArchive = archive;
	return _Init();
}
