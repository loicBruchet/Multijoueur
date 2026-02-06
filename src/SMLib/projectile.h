#pragma once
#include "framework.h"
class Projectile
{
public:
	Projectile(){}
	~Projectile(){}

	void Create(cpu_mesh* pMesh, cpu_material* pMaterial, XMFLOAT3 pos, XMFLOAT3 dir);
	void Destroy();
	void Update();

	cpu_entity* GetEntity() { return m_pEntity; }
	cpu_entity* m_pPlayerEntityUser;
protected:
	cpu_entity* m_pEntity;
};

