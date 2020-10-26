// Fill out your copyright notice in the Description page of Project Settings.


#include "KTcpSocketConnection.h"

AKTcpSocketConnection::~AKTcpSocketConnection()
{
	FString inString(TEXT("ConnectClose "));
	uint32 size = inString.Len();
	TArray<uint8> data;
	data.AddUninitialized(size);
	StringToBytes(inString, data.GetData(), size);
	
	SendData(ConnectedSocket, data);

	FString inString2(TEXT("@%&#$"));
	uint32 size2 = inString2.Len();
	TArray<uint8> data2;
	data2.AddUninitialized(size2);
	StringToBytes(inString2, data2.GetData(), size2);

	SendData(ConnectedSocket, data2);
}
