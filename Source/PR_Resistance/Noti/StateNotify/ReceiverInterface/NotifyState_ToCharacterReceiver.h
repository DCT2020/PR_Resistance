// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NotifyState_ToCharacterReceiver.generated.h"
/**
 * 
 */
UENUM()
enum class EAnimNotifyToCharacterTypes : uint8
{
	ATC_ATTACK,
	ATC_RELOAD_COMPLITE,
};

 // This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNotifyState_ToCharacterReceiver : public UInterface
{
	GENERATED_BODY()
};

class PR_RESISTANCE_API INotifyState_ToCharacterReceiver
{
	GENERATED_BODY()
 public:
    virtual void ReceiveNotification(EAnimNotifyToCharacterTypes curNotiType, bool bIsEnd) = 0;
};
