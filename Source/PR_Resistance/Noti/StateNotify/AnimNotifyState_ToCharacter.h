// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

// interface
#include "PR_Resistance/Noti/StateNotify/ReceiverInterface/NotifyState_ToCharacterReceiver.h"

#include "AnimNotifyState_ToCharacter.generated.h"

/**
 * 
 */

 /*
    애니메이션에서의 대부분의 Notification을 담당
    받는 Actor는 INotifyState_ToCharacterReceiver를 상속받아야 함.
    구분은 인터페이스에 있는 EAnimNotifyToCharacterTypes을 통해 수행함
 */
UCLASS()
class PR_RESISTANCE_API UAnimNotifyState_ToCharacter : public UAnimNotifyState
{
	GENERATED_BODY()

    // from UAnimNotifyState inherited
    void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

 private:
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
    EAnimNotifyToCharacterTypes mType;
};
