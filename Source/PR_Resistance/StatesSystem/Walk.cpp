// Fill out your copyright notice in the Description page of Project Settings.


#include "Walk.h"

Walk::Walk(FStatus const* status)
	:IState(status)
{
}

Walk::~Walk()
{
}

bool Walk::Init()
{

	return true;
}

bool Walk::Begin()
{
	
	return true;
}

void Walk::Update(float deltaTime)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, TEXT("CurState : Walk"));
}

void Walk::End()
{
}

int Walk::GetPriority()
{
	return 0;
}
