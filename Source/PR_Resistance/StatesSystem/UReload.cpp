#include "UReload.h"
#include "Engine/DataTable.h"
#include "PR_Resistance/Chara/PR_ResistanceCharacter.h"


UReload::UReload()
{
	mDesc.StateType = (uint8)CharacterState::CS_SUB_RELOAD;
	mDesc.Priority = 3;
}

bool UReload::Begin(uint8 prevState)
{
	mAnimInstance->PlaySlotAnimationAsDynamicMontage(mReloadAnimationData->mAnimation, "UpperMotion");

	mFloats->Set(mStatus->MaxAmmo,1);

	return true;
}

void UReload::Update(float deltaTime)
{
}

void UReload::End()
{
	
}

bool UReload::_Init()
{
	void* buffer = nullptr;
	GetCharaDataWithLog("AnimTable", &buffer);
	{
		UDataTable* animTable = static_cast<UDataTable*>(buffer);
		mReloadAnimationData = animTable->FindRow<FCharacterAnimationData>(TEXT("Reload"), nullptr);
	}
	GetCharaDataWithLog("AnimInstance", &buffer);
	{
		mAnimInstance = static_cast<UAnimInstance*>(buffer);
		mAnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UReload::OnAnimEnd);
	}
	GetCharaDataWithLog("Floats", &buffer);
	{
		mFloats = static_cast<UFloatsComponent*>(buffer);
	}
	GetCharaDataWithLog("Status", &buffer);
	{
		mStatus = static_cast<FStatus*>(buffer);
	}
	
	return true;
}

void UReload::OnAnimEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	mDesc.bIsEnd = true;
}
