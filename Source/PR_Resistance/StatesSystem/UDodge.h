// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "PR_Resistance/Interface/IStaminaUser.h"
#include "UDodge.generated.h"
/**
 *	TODO: ���ݺҰ����� �ڵ��ؾ���
 */
 UCLASS()
class PR_RESISTANCE_API UDodge : public UCState, public IStaminaUser
{
GENERATED_BODY()
protected:
	IStaminaProvider* mSPProvider = nullptr;
	FStatus* mChracterStatus = nullptr;

	//�ӽ�
	float* mMaxWalkSpeed = nullptr;
	float mElapsedTime = 0.0f;

	bool _Init() override;

public:
	UDodge();
	virtual ~UDodge();

	bool Begin(uint8 prevState) override;
	void Update(float deltaTime) override;
	void End() override;

	void SetProvider(IStaminaProvider* provider) override;
};
