#pragma once
#include "framework.h"

class Control
{
public:
	Control() {}
	~Control() {}

	void Moved(PacketsWriter writer, NetPlayerOut* m_netPacket, ThreadData* datas, SOCKET m_pData);
	void Update(Player* pPlayer, NetPlayerOut* m_netPacket);

protected:
	PacketsWriter writer;

	ThreadData* datas;

	SOCKET m_pData;

	POINT p;
	POINT newp;
};

