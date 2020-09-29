// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SHA256_.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PR_RESISTANCE_API USHA256_ : public UObject
{
	GENERATED_BODY()
public:
    
    UFUNCTION(BlueprintCallable)
    FString ToHash(FString string);
};
