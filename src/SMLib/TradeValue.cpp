#include "pch.h"
#include "TradeValue.h"

cpu_transform TradeValue::TradeFloatPacketToTransform(NetPlayerOut* nPO, cpu_transform ent)
{
	ent.pos.x = nPO->posx;
	ent.pos.y = nPO->posy;
	ent.pos.z = nPO->posz;
	ent.dir.x = nPO->directionX;
	ent.dir.y = nPO->directionY;
	ent.dir.z = nPO->directionZ;
	ent.right.x = nPO->rightX;
	ent.right.y = nPO->rightY;
	ent.right.z = nPO->rightZ;
	ent.up.x = nPO->upX;
	ent.up.y = nPO->upY;
	ent.up.z = nPO->upZ;
	return ent;
}

int TradeValue::GetInputAction(char c)
{
	if (Layout == Keyboardlayout::QWERTY) {
		if (c == 87)
			return PlayerActions::ACTION_FORWARD;  // W
		if (c == 65)
			return PlayerActions::ACTION_LEFT;     // A
	}
	else { // AZERTY
		if (c == 90)
			return PlayerActions::ACTION_FORWARD;  // Z
		if (c == 81)
			return PlayerActions::ACTION_LEFT;     // Q
	}
	switch (c) {
	case 83:
		return PlayerActions::ACTION_BACKWARD; // S
	case 68:
		return PlayerActions::ACTION_RIGHT;    // D
	case 80:
		if (layoutChangeCooldown >= 0.5f)
		{
			Layout = (Layout == Keyboardlayout::AZERTY) ? Keyboardlayout::QWERTY : Keyboardlayout::AZERTY;
			layoutChangeCooldown = 0.0f;
		}
		return PlayerActions::ACTION_NONE;      // P
	case 32:
		return PlayerActions::ACTION_UP;       // SPACE
	default:
		return PlayerActions::ACTION_NONE;
	}
}