#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "UReload.generated.h"

class UFloatsComponent;
struct FCharacterAnimationData;
UCLASS()
class UReload : public UCState
{
	GENERATED_BODY()
public:
	UReload();

	bool Begin(uint8 prevState) override;
	void Update(float deltaTime) override;
	void End() override;

protected:
	bool _Init() override;

protected:
	FCharacterAnimationData* mReloadAnimationData = nullptr;
	UAnimInstance* mAnimInstance = nullptr;
	UFloatsComponent* mFloats = nullptr;
	FStatus* mStatus = nullptr;

	UFUNCTION()
		void OnAnimEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};