#pragma once
#include "framework.h"

class TradeValue 
{
public:
	cpu_transform TradeFloatPacketToTransform(NetPlayerOut* nPO, cpu_transform ent);
	int GetInputAction(char c);
	int Layout = Keyboardlayout::AZERTY;

	float layoutChangeCooldown = 0.0f;
};