// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TcpSocketConnection.h"
#include "KTcpSocketConnection.generated.h"

/**
 * 
 */
UCLASS()
class PR_RESISTANCE_API AKTcpSocketConnection : public ATcpSocketConnection
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		int ConnectedSocket = 0;
public:
	virtual ~AKTcpSocketConnection();
};
