// Fill out your copyright notice in the Description page of Project Settings.


#include "CState.h"

UCState::UCState()
{
}

UCState ::~UCState()
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

bool UCState::Begin(uint8 prevState)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void UCState::Update(float deltaTime)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void UCState::End()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool UCState::_Init()
{
	throw std::logic_error("The method or operation is not implemented.");
}

UCharacterDataArchive* UCState::GetCharacterDataArchive()
{
	return mDataArchive;
}

bool UCState::Init(UCharacterDataArchive* archive)
{
	if (archive == nullptr)
	{
		return false;
	}

	mDataArchive = archive;
	return _Init();
}
