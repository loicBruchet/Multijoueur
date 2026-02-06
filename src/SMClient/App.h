#pragma once
#include "pch.h"
#include "Control.h"
#ifdef _DEBUG
#define WINDIVIDER 3
#else
#define WINDIVIDER 1
#endif
class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);

	void SpawnMissile(XMFLOAT3 pos, XMFLOAT3 dir, Player* pl);
	void OnCollision();

	static DWORD WINAPI SendMessCo(LPVOID lpParam);
	static DWORD WINAPI SendInput(LPVOID lpParam);
	static DWORD WINAPI ReceiveData(LPVOID lpParam);

private:
	inline static App* s_pApp = nullptr;

	WSADATA wsaData;
	//ressources
	cpu_font m_font;
	cpu_font m_font2;
	cpu_texture m_spriteTexture;
	cpu_sprite* m_sprite;
	ThreadData* m_pData;
	PlayerData* m_plData;
	NetPlayerOut* m_netPacket;
	PacketsWriter writer;

	cpu_texture m_pEarthTexture;
	cpu_material materialPlanet;

	HANDLE h1;
	HANDLE h2;
	HANDLE h3;

	bool m_isRunning = true;
	bool IPDone = false;

	float time = 0;

	std::vector<uint8_t> buffer;

	cpu_mesh m_meshPlayer;
	cpu_mesh m_meshSpher;
	cpu_mesh m_meshMissile;

	Control m_control;
	//Shader
	cpu_material m_materialPlayer;
	cpu_material m_materialMissile;

	// 3D
	Map* m_map;
	TradeValue TrdValue;
	Player* m_player;
	std::vector<Player*> m_entities; 
	std::vector<Projectile*> m_missiles; 
	cpu_particle_emitter* m_pEmitter;
	void MissileShader(cpu_ps_io& io);
};