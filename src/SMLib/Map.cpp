#include "pch.h"
#include "Map.h"
Map::Map()
{
	m_pPlutonTexture.Load("../../res/Textures/pluton.png");
	m_pUranusTexture.Load("../../res/Textures/uranus.png");
	m_pSunTexture.Load("../../res/Textures/sun.png");
	m_pIriseeTexture.Load("../../res/Textures/lune_irise.png");
	m_pJupiterTexture.Load("../../res/Textures/jupiter.png");
	m_pMeteoreTexture.Load("../../res/Textures/meteore.png");
	m_pMercureTexture.Load("../../res/Textures/mercure.png");

	pMaterialUranus.pTexture = &m_pUranusTexture;
	pMaterialPluton.pTexture = &m_pPlutonTexture;
	pMaterialSun.pTexture = &m_pSunTexture;
	pMaterialIrisee.pTexture = &m_pIriseeTexture;
	pMaterialJupiter.pTexture = &m_pJupiterTexture;
	pMaterialMeteore.pTexture = &m_pMeteoreTexture;
	pMaterialMercure.pTexture = &m_pMercureTexture;


	m_meshPluton.CreateSphere(3.0f, 10, 10);
	m_meshUranus.CreateSphere(6.0f, 20, 20);
	m_meshSun.CreateSphere(15.0f, 20, 20);
	m_meshIrisee.CreateSphere(4.5f, 10, 10);
	m_meshJupiter.CreateSphere(10.5f, 20, 20);
	m_meshMeteore.CreateSphere(2.4f, 1, 2);
	m_meshMercure.CreateSphere(8.4f, 20, 20);
}

Map::~Map()
{
}

void Map::Update()
{
	float dt = cpuTime.delta;
	float time = cpuTime.total;
}
void Map::CreateMap()
{
	CreatePlanet(XMFLOAT3{ 8, 12, -6 }, &m_meshSun, &pMaterialSun);
	CreatePlanet(XMFLOAT3{ -32, -18, 14 }, &m_meshSun, &pMaterialSun);
	CreatePlanet(XMFLOAT3{ 28, -22, 30 }, &m_meshSun, &pMaterialSun);
	CreatePlanet(XMFLOAT3{ -10, 35, -28 }, &m_meshSun, &pMaterialSun);

	CreatePlanet(XMFLOAT3{ 40, 10, -12 }, &m_meshJupiter, &pMaterialJupiter);
	CreatePlanet(XMFLOAT3{ -38, 22, -6 }, &m_meshJupiter, &pMaterialJupiter);
	CreatePlanet(XMFLOAT3{ 14, -40, -8 }, &m_meshJupiter, &pMaterialJupiter);
	CreatePlanet(XMFLOAT3{ -6, -6, 38 }, &m_meshJupiter, &pMaterialJupiter);

	CreatePlanet(XMFLOAT3{ 22, 48, 7 }, &m_meshMercure, &pMaterialMercure);
	CreatePlanet(XMFLOAT3{ -38, 6, 34 }, &m_meshMercure, &pMaterialMercure);
	CreatePlanet(XMFLOAT3{ 6, -28, 22 }, &m_meshMercure, &pMaterialMercure);
	CreatePlanet(XMFLOAT3{ -34, -30, -10 }, &m_meshMercure, &pMaterialMercure);
	CreatePlanet(XMFLOAT3{ 40, 8, 30 }, &m_meshMercure, &pMaterialMercure);

	CreatePlanet(XMFLOAT3{ -12, 42, 12 }, &m_meshUranus, &pMaterialUranus);
	CreatePlanet(XMFLOAT3{ 32, 34, -18 }, &m_meshUranus, &pMaterialUranus);
	CreatePlanet(XMFLOAT3{ -42, 30, 20 }, &m_meshUranus, &pMaterialUranus);
	CreatePlanet(XMFLOAT3{ 18, 10, 40 }, &m_meshUranus, &pMaterialUranus);

	CreatePlanet(XMFLOAT3{ -26, 12, -38 }, &m_meshIrisee, &pMaterialIrisee);
	CreatePlanet(XMFLOAT3{ 6, 38, 24 }, &m_meshIrisee, &pMaterialIrisee);
	CreatePlanet(XMFLOAT3{ 34, 6, -34 }, &m_meshIrisee, &pMaterialIrisee);
	CreatePlanet(XMFLOAT3{ -6, -42, 14 }, &m_meshIrisee, &pMaterialIrisee);

	CreatePlanet(XMFLOAT3{ 2, -6, -42 }, &m_meshMeteore, &pMaterialMeteore);
	CreatePlanet(XMFLOAT3{ -18, -14, 42 }, &m_meshMeteore, &pMaterialMeteore);
	CreatePlanet(XMFLOAT3{ 42, 18, 10 }, &m_meshMeteore, &pMaterialMeteore);

	CreatePlanet(XMFLOAT3{ -22, 30, -14 }, &m_meshPluton, &pMaterialPluton);
	CreatePlanet(XMFLOAT3{ 12, -38, 41 }, &m_meshPluton, &pMaterialPluton);
	CreatePlanet(XMFLOAT3{ 38, 22, -4 }, &m_meshPluton, &pMaterialPluton);
	CreatePlanet(XMFLOAT3{ -8, -34, -26 }, &m_meshPluton, &pMaterialPluton);
	CreatePlanet(XMFLOAT3{ 4, 44, -30 }, &m_meshPluton, &pMaterialPluton);
	CreatePlanet(XMFLOAT3{ -44, 8, -28 }, &m_meshPluton, &pMaterialPluton);

	m_pSpawnPosition.push_back(XMFLOAT3{ 0,3,-30 });
	m_pSpawnPosition.push_back(XMFLOAT3{ 20,-40,10 });
	m_pSpawnPosition.push_back(XMFLOAT3{ -35,35,15 });
	m_pSpawnPosition.push_back(XMFLOAT3{ 0,10,15 });
}

void Map::CreatePlanet(XMFLOAT3 pPos, cpu_mesh* pMesh, cpu_material* pMaterial)
{
	cpu_entity* m_Entity = cpuEngine.CreateEntity();
	m_Entity->pMesh = pMesh;
	m_Entity->pMaterial = pMaterial;
	m_Entity->transform.pos = pPos;
	entitiesMap.push_back(m_Entity);
}
