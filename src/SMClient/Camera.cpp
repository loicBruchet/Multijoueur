#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	Spawning = true;
}
Camera::~Camera()
{

}
void Camera::Update(float _x, float _y, float _z, Player* pPlayer)
{
	if (pPlayer->GetState() == 2)
	{
		XMFLOAT3 pos = pPlayer->GetEntity()->transform.pos;
		if (Spawning == true)
		{
			cpuEngine.GetCamera()->transform.pos = pos;
			cpuEngine.GetCamera()->transform.pos.y += 1;
			cpuEngine.GetCamera()->transform.rot = pPlayer->GetEntity()->transform.rot;
			cpuEngine.GetCamera()->transform.dir = pPlayer->GetEntity()->transform.dir;
			Spawning = false;
		}
	}
	else
	{
		Spawning = true;
		cpuEngine.GetCamera()->transform.pos = pPlayer->GetEntity()->transform.pos;
		cpuEngine.GetCamera()->transform.dir = pPlayer->GetEntity()->transform.dir;
		cpuEngine.GetCamera()->transform.rot = pPlayer->GetEntity()->transform.rot;
	}
	//cpuEngine.GetCamera()->transform.LookAt(pMesh->transform.pos.x, pMesh->transform.pos.y, pMesh->transform.pos.z);
	//cpuEngine.GetCamera()->transform.pos = pMesh->transform.pos;
}
