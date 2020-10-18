// Fill out your copyright notice in the Description page of Project Settings.


#include "Aim.h"

UAim::UAim()
{
	mDesc.StateType = (uint8)CharacterState::CS_SUB_AIM;
	mDesc.Priority = 3;
}

bool UAim::_Init()
{
	return true;
}

bool UAim::Begin(uint8 prevState)
{
	return true;
}

void UAim::Update(float deltaTime)
{
}

void UAim::End()
{
}

