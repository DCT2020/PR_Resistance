// Fill out your copyright notice in the Description page of Project Settings.


#include "UJump.h"

UJump::UJump()
{
	mDesc.StateType = (uint8)CharacterState::CS_JUMP;
	mDesc.Priority = 4;
}

UJump::~UJump()
{
}

bool UJump::Begin(uint8 prevState)
{
	return true;
}

void UJump::Update(float deltaTime)
{
}

void UJump::End()
{
}

bool UJump::_Init()
{
	return true;
}
