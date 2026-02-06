#include "pch.h"


void Projectile::Create(cpu_mesh* pMesh, cpu_material* pMaterial, XMFLOAT3 pos, XMFLOAT3 dir)
{
	m_pEntity = cpuEngine.CreateEntity();
	m_pEntity->pMesh = pMesh;
	m_pEntity->pMaterial = pMaterial;
	m_pEntity->transform.pos = pos;
	m_pEntity->transform.SetScaling(0.2f);
	m_pEntity->transform.dir = dir;
	m_pEntity->transform.Move(2.5f);
}