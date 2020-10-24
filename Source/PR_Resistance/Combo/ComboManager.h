// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Animation/AnimInstance.h"
#include "PR_Resistance/Combo/Action.h"

#include "UObject/UObject.h"

#include <functional>
/**
 * 
 */
class PR_RESISTANCE_API ComboManager
{
public:
	ComboManager();
	~ComboManager();
protected:
	TMap<FName, const FAction*> Actions;
	TMap<ActionInput, const FAction*> StartActions;
	const FAction* mCurAction = nullptr;
	UAnimInstance* mOwnerAnimInst = nullptr;
	UAnimMontage* mCurDynmMontage = nullptr;
	std::function<void()> ComboEndEvent;

	bool mbIsWait = false;
	float mCurWaitTime = 0.0f;
	float mEalsedTime = 0.0f;
public:

	void Init(UDataTable* actionTable, UAnimInstance* animInstance);
	void Update(float deltaTime);
	bool StartAttack(FName firstAttack);
	void StartWaitInput();
	void PushInput(ActionInput inputType);
	void BindComboAndEvent(std::function<void()> function);
	void SetComboEnd();

	UFUNCTION(NetMulticast)
		void PlaySlotAnimation(FName slotName, UAnimSequenceBase* animSequence);
		void PlaySlotAnimation_Implementation(FName slotName, UAnimSequenceBase* animSequence);

	UFUNCTION(NetMulticast)
		void StopSlotAnimation(FName slotName);
		void StopSlotAnimation_Implementation(FName slotName);

protected:
	bool ChangeAction(FName actionName);
	bool ChangeAction(const FAction* action);
};
