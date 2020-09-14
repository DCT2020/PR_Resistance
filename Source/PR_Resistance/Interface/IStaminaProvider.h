#pragma once


class IStaminaProvider
{
public:
	virtual bool UseStamina(float usedStamina) = 0;
};