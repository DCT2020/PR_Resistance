// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_ToCharacter.h"

void UAnimNotifyState_ToCharacter::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	INotifyState_ToCharacterReceiver* noti = Cast<INotifyState_ToCharacterReceiver>(MeshComp->GetOwner());
	if (noti != nullptr)
	{
		noti->ReceiveNotification(mType, false);
	}
}

void UAnimNotifyState_ToCharacter::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	INotifyState_ToCharacterReceiver* noti = Cast<INotifyState_ToCharacterReceiver>(MeshComp->GetOwner());
	if (noti != nullptr)
	{
		noti->ReceiveNotification(mType, true);
	}
}
