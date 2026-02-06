#pragma once
#include "framework.h"
class Camera
{
	int m_state;
	//cpu_camera* m_camera;
	bool Spawning;
public:
	int id;
	Camera();
	~Camera();
	void Update(float _x, float _y, float _z, Player* pPlayer);
};

