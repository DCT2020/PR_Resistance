// Fill out your copyright notice in the Description page of Project Settings.

#include "UIdle.h"

UIdle::UIdle()
{
	temp.StateType = CharacterState::CS_IDLE;
	temp.Priority = 1;
}

UIdle::~UIdle()
{
	
}

bool UIdle::Init()
{

	return true;
}

void UIdle::Update(float deltaTime)
{
	//if (GEngine && bIsCanDebug)
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, TEXT("CurState : Idle"));
	//	bIsCanDebug = false;
	//}
}

bool UIdle::Begin()
{
	return true;
}

void UIdle::End()
{
}

FStateDesc UIdle::GetDesc()
{
	return temp;
}

