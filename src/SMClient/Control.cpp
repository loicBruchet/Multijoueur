#pragma once
#include "pch.h"

void Control::Moved(PacketsWriter writer, NetPlayerOut* m_netPacket, ThreadData* datas, SOCKET m_pData)
{
	this->writer = writer;
	this->datas = datas;
	this->m_pData = m_pData;
	writer.WriteDataClient(m_netPacket, datas, m_pData);
}

void Control::Update(Player* pPlayer, NetPlayerOut* m_netPacket)
{

	if (pPlayer->GetState() == 2)
		return;
	float dt = cpuTime.delta;

	POINT current;
	GetCursorPos(&current);
	float midCursor = 0.5f;
	int centerx = 1920 * midCursor;
	int centery = 1080 * midCursor;

	float deltax = (float)(current.x - centerx) * 0.1;
	float deltay = (float)(current.y - centery) * 0.1;

	double magnitude = hypot(deltax, deltay);
	//// Move ship
	float speed = 2.0f;
	GetCursorPos(&newp);

	if (deltax != 0 || deltay != 0)
	{
		pPlayer->GetEntity()->transform.AddYPR(deltax / magnitude * dt * speed, deltay / magnitude * dt* speed, 0/*(ray.dir.z - newRay.dir.z)*2*dt*/);
		cpuEngine.GetCamera()->transform.AddYPR(deltax / magnitude * dt * speed, deltay / magnitude * dt * speed, 0/*(ray.dir.z - newRay.dir.z)*2*dt*/);
	}
	cpuEngine.GetCamera()->transform.pos = pPlayer->GetEntity()->transform.pos;

	m_netPacket->directionX = cpuEngine.GetCamera()->transform.dir.x;
	m_netPacket->directionY = cpuEngine.GetCamera()->transform.dir.y;
	m_netPacket->directionZ = cpuEngine.GetCamera()->transform.dir.z;
	m_netPacket->rightX = cpuEngine.GetCamera()->transform.right.x;
	m_netPacket->rightY = cpuEngine.GetCamera()->transform.right.y;
	m_netPacket->rightZ = cpuEngine.GetCamera()->transform.right.z;
	m_netPacket->upX = cpuEngine.GetCamera()->transform.up.x;
	m_netPacket->upY = cpuEngine.GetCamera()->transform.up.y;
	m_netPacket->upZ = cpuEngine.GetCamera()->transform.up.z;
	m_netPacket->posx = cpuEngine.GetCamera()->transform.pos.x;
	m_netPacket->posy = cpuEngine.GetCamera()->transform.pos.y;
	m_netPacket->posz = cpuEngine.GetCamera()->transform.pos.z;


	SetCursorPos(centerx, centery);
	GetCursorPos(&p);
	m_netPacket->action = PlayerActions::ACTION_NONE;
	if (current.x != p.x || current.y != current.y)
		Moved(writer, m_netPacket, datas, m_pData);

}