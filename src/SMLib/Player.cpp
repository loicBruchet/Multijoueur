#include "pch.h"



Player::Player()
{
	m_pEntity = nullptr;
	m_pFSM = nullptr;
	Health = 50;
}

Player::~Player()
{
}

void Player::Create(cpu_mesh* pMesh, cpu_material* pMaterial)
{
	m_pEntity = cpuEngine.CreateEntity();
	m_pEntity->pMesh = pMesh;
	m_pEntity->pMaterial = pMaterial;

	m_pFSM = cpuEngine.CreateFSM(this);
	m_pFSM->SetGlobal<StatePlayerGlobal>();
	m_pFSM->Add<StatePlayerIdle>();
	m_pFSM->Add<StatePlayerBlink>();
	m_pFSM->Add<StatePlayerSpawn>();

	cpuEngine.GetParticleData()->Create(300000);
	cpuEngine.GetParticlePhysics()->gy = -0.5f;
	m_pEmitter = cpuEngine.CreateParticleEmitter();
	m_pEmitter->density = 30.0f;
	m_pEmitter->colorMax = cpu::ToColor(255, 126, 0);
	m_pEmitter->colorMin = cpu::ToColor(217, 0, 43);
	m_pEmitter->pos = m_pEntity->transform.pos;
}

void Player::Destroy()
{
	m_pFSM = cpuEngine.Release(m_pFSM);
	m_pEntity = cpuEngine.Release(m_pEntity);
}

void Player::Update()
{
	m_pEmitter->pos = m_pEntity->transform.pos;
	m_pEmitter->dir = m_pEntity->transform.dir;
	m_pEmitter->dir.x = -m_pEmitter->dir.x;
	m_pEmitter->dir.y = -m_pEmitter->dir.y;
	m_pEmitter->dir.z = -m_pEmitter->dir.z;
	if (revive && state != 2)
	{
		m_pFSM->ToState(CPU_ID(StatePlayerSpawn));
	}
}

void Player::Revive()
{
	revive != revive;
}