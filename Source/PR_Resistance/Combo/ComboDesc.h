// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/Combo/Action.h"
#include "Engine/DataTable.h"
#include "ComboDesc.generated.h"

/**
 * 
 */
 USTRUCT(Atomic,BlueprintType)
struct PR_RESISTANCE_API FComboDesc : public FTableRowBase
{
    GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAction Action;
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TArray<FAction> NextCombos;
};
