// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(PRR, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(PRR,Verbosity,TEXT("%s"), *ABLOG_CALLINFO)																// 파일 이름, 함수, 라인 정보를 추가해 로그
#define ABLOG(Verbosity, Format, ...) UE_LOG(PRR, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))		// ABLOG_S에 형식 문자열로 추가 정보를 지정해 로그

