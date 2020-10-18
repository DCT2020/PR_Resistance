// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
// in engine headers
#include "Components/ActorComponent.h"

// my headers
#include "PR_Resistance/Component/FloatsComponent.h"
#include "Containers/ContainerAllocationPolicies.h"

#include "Destructible_Comp.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PR_RESISTANCE_API UDestructible_Comp : public UActorComponent, public IFloatListener
{
	GENERATED_BODY()

public:	
	// statics
	static const int MAX_STAGE = 3;

	// Sets default values for this component's properties
	UDestructible_Comp(const FObjectInitializer& ObjectInitializer);

	UStaticMeshComponent* mOwnerStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = Destructible, meta = (DisplayName = "Destructible State Stage HPs"))
	float mHPs[3];

	UPROPERTY(EditAnywhere, Category = Destructible, meta = (DisplayName = "Normal Shape"))
	UStaticMesh* mNormalShape;

	UPROPERTY(EditAnywhere, Category = Destructible, meta = (DisplayName = "Second Shape"))
	UStaticMesh* mSecondShape;

	UPROPERTY(EditAnywhere, Category = Destructible, meta = (DisplayName = "Last Shape"))
	UStaticMesh* mLastShape;

	int mCurStage = 0;
	UStaticMesh* mStageShapes[MAX_STAGE];

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public: // native call
	void Init(UStaticMeshComponent* targetStateMesh, UFloatsComponent* floatcomp, uint8 indexOfHpInFloatsComp);
	void ListenFloat(int index, float newFloat) override;
public: // blueprint call
	
	UFUNCTION(BlueprintCallable, Category = Destructible, meta = (DisplayName = "Init"))
	void Init_bp(const UStaticMeshComponent* targetStateMesh, const UFloatsComponent* FloatComponent, const uint8 indexOfHpInFloatsComp);

protected:
	int mIndexOfHp = 0;
};
