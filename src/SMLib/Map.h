#pragma once
#include "framework.h"

class Map
{
public:
	Map();
	~Map();
	void Update();
	void CreateMap();
	
	std::vector<cpu_entity*> entitiesMap;
	std::vector<XMFLOAT3> m_pSpawnPosition;

protected:

	void CreatePlanet(XMFLOAT3 pPos, cpu_mesh* pMesh, cpu_material* pMaterial);

	cpu_material pMaterialBarrier;
	cpu_material pMaterialUranus;
	cpu_material pMaterialPluton;
	cpu_material pMaterialSun;
	cpu_material pMaterialIrisee;
	cpu_material pMaterialJupiter;
	cpu_material pMaterialMeteore;
	cpu_material pMaterialMercure;

	cpu_mesh m_meshBarrier;
	cpu_mesh m_meshUranus;
	cpu_mesh m_meshPluton;
	cpu_mesh m_meshSun;
	cpu_mesh m_meshIrisee;
	cpu_mesh m_meshJupiter;
	cpu_mesh m_meshMeteore;
	cpu_mesh m_meshMercure;

	cpu_texture m_pBarrierTexture;
	cpu_texture m_pUranusTexture;
	cpu_texture m_pSunTexture;
	cpu_texture m_pPlutonTexture;
	cpu_texture m_pIriseeTexture;
	cpu_texture m_pJupiterTexture;
	cpu_texture m_pMeteoreTexture;
	cpu_texture m_pMercureTexture;
};