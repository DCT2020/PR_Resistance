// Fill out your copyright notice in the Description page of Project Settings.


#include "UWalk.h"

UWalk::UWalk()
{
	temp.StateType = CharacterState::CS_WALK;
	temp.Priority = 2;
}

UWalk::~UWalk()
{
}

bool UWalk::Init()
{

	return true;
}

bool UWalk::Begin()
{
	
	return true;
}

void UWalk::Update(float deltaTime)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, TEXT("CurState : Walk"));
}

void UWalk::End()
{
}

FStateDesc UWalk::GetDesc()
{
	
	return temp;
}
