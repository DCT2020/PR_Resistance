#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <functional>

#include "SlotMotionProcess.generated.h"

USTRUCT()
struct PR_RESISTANCE_API FSlotMotionProcess
{
	GENERATED_BODY()
		std::function<void()> mLamda;
};

USTRUCT()
struct PR_RESISTANCE_API FSlotMotionQueue
{
	GENERATED_BODY()
		TArray<FSlotMotionProcess> mSlotMotionFuncs;
};

