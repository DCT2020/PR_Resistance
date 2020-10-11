// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FloatsComponent.generated.h"


class IFloatListener
{
public:
	virtual void ListenFloat(float newFloat) = 0;
};

UCLASS( ClassGroup=(Float), meta=(BlueprintSpawnableComponent) )
class PR_RESISTANCE_API UFloatsComponent : public UActorComponent
{
	GENERATED_BODY()

	TArray<TArray<IFloatListener*>*> mListeners;
	TArray<float> mFloats;

public:	
	// Sets default values for this component's properties
	UFloatsComponent();
	~UFloatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	//native call
	void	MakeFloats(uint8 size); // size ���� ��ŭ float�� ���� �մϴ�. 0 ~ (size -1), ������ 0 ~ (size - 1) ������ �� ���� �����˴ϴ�.
	bool	AddListener(IFloatListener* newFloatListener, uint8 index); // index�� �ش�Ǵ� float�� �������� ������ �����մϴ�.
	
	int		PushBack(float newValue); // ����� ��ȯ�մϴ�.

	int		GetSize();
	bool	Set(const float newValue, uint8 index);		// index�� �ش��ϴ� float�� �������� ������ false ��ȯ
	bool	Get(uint8 index, float& out);				// index�� �ش��ϴ� float�� �������� ������ false ��ȯ
public: //blueprint call

	UFUNCTION(BlueprintCallable, Category = Floats, meta = (DisplayName = "MakeFloats"))
	void MakeFloats_bp(uint8 size);

	UFUNCTION(BlueprintCallable, Category = Floats, meta = (DisplayName = "PushBack"))
		void PushBack_bp(float newValue, int& size);

	UFUNCTION(BlueprintCallable, Category = Floats, meta = (DisplayName = "Get"))
	void Get_bp(uint8 index, float& value, bool& isValid);

	UFUNCTION(BlueprintCallable, Category = Floats, meta = (DisplayName = "Set"))
	void Set_bp(float newValue, uint8 index, bool& isValid);

};