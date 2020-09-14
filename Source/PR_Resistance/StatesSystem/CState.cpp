// Fill out your copyright notice in the Description page of Project Settings.


#include "CState.h"

CState::CState()
{
}

CState::~CState()
{
}

FStateDesc CState::GetStateDesc()
{
	return mDesc;
}

void CState::SetStart()
{
	mDesc.bIsEnd = false;
}

void CState::SetStop()
{
	mDesc.bIsEnd = true;
}

CharacterDataArchive * const CState::GetCharacterDataArchive()
{
	return mDataArchive;
}

bool CState::Init(CharacterDataArchive* archive)
{	
	if (archive == nullptr)
	{
		return false;
	}

	mDataArchive = archive;
	return _Init();
}
