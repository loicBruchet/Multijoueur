#pragma once
#include "framework.h"
class Player
{
public:
	int GetHealth() { return Health; }
	int GetPoints() { return Points; }
	int GetState() { return state; }
	int GetID() { return id; }

	void SetHealth(int h) { Health = h; }
	void SetPoints(int p) { Points = p; }
	void SetState(int s) { state = s; }
	void SetID(int i) { id = i; }

	Player();
	~Player();

	void Create(cpu_mesh* pMesh, cpu_material* pMaterial);
	void Destroy();
	void Revive();
	void Update();

	cpu_entity* GetEntity() { return m_pEntity; }

	cpu_ray GetRay() { return pos; }

	cpu_hit GetHit() { return m_pHitEntity; }

	cpu_fsm<Player>* GetFSM() { return m_pFSM; }

	std::vector<XMFLOAT3> m_pSpawnPosition;
protected:
	cpu_particle_emitter* m_pEmitter;

	POINT p;
	POINT newp;

	cpu_ray pos;

	cpu_entity* m_pEntity;

	cpu_fsm<Player>* m_pFSM;

	cpu_hit m_pHitEntity;

	bool revive = false;

	int Health = 0;
	int Points = 0;
	int state;
	int id;
};

