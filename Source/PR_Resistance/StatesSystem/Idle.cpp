// Fill out your copyright notice in the Description page of Project Settings.

#include "Idle.h"

Idle::Idle(FStatus const* status)
	:IState(status)
{

}

Idle::~Idle()
{
	
}

bool Idle::Init()
{
	return true;
}

void Idle::Update(float deltaTime)
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Blue, TEXT("CurState : Idle"));
}

bool Idle::Begin()
{
	return true;
}

void Idle::End()
{
}

int Idle::GetPriority()
{
	return 0;
}

