// Fill out your copyright notice in the Description page of Project Settings.

#include "UIdle.h"

UIdle::UIdle()
{
	mDesc.StateType = (uint8)CharacterState::CS_IDLE;
	mDesc.Priority = 1;
}

UIdle::~UIdle()
{
	
}

bool UIdle::_Init()
{

	return true;
}

void UIdle::Update(float deltaTime)
{
}

bool UIdle::Begin(uint8 prevState)
{
	return true;
}

void UIdle::End()
{
}
