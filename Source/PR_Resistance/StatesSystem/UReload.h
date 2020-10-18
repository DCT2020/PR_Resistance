#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "UReload.generated.h"

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
	float elapsedTime = 0.0f;
};