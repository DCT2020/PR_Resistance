// Fill out your copyright notice in the Description page of Project Settings.


#include "SHA256_.h"
#include "PR_Resistance/SHA256.h"

FString USHA256_::ToHash(FString string)
{
	return sha256(string);
}
