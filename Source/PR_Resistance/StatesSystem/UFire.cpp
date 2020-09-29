// Fill out your copyright notice in the Description page of Project Settings.


#include "UFire.h"

UFire::UFire()
{
	mDesc.Priority = 3;
	mDesc.StateType = CharacterState::CS_ATTACK;
}

UFire::~UFire()
{
}

bool UFire::_Init()
{
	return true;
}

bool UFire::Begin(CharacterState prevState)
{
	return true;
}

void UFire::Update(float deltaTime)
{
	
}

void UFire::End()
{
	
}

void UFire::SetProvider(IStaminaProvider* provider)
{
	mPProvider = provider;
}
