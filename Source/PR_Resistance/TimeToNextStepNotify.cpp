// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeToNextStepNotify.h"
#include "PR_Resistance/Noti/Notify/ITimeToNextStepNotify.h"

void UTimeToNextStepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	IITimeToNextStepNotify* character = Cast<IITimeToNextStepNotify>(MeshComp->GetOwner());
	if (character != nullptr)
	{
		character->StartWait();
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("%d is not IITimeToNextStepNotify"),__LINE__)
	}
}
