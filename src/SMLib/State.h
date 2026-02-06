#pragma once
#include "framework.h"
struct StatePlayerGlobal
{
	void OnEnter(Player& cur, int from);
	void OnExecute(Player& cur);
	void OnExit(Player& cur, int to);
};

struct StatePlayerIdle
{
	void OnEnter(Player& cur, int from);
	void OnExecute(Player& cur);
	void OnExit(Player& cur, int to);
};

struct StatePlayerSpawn
{
	void OnEnter(Player& cur, int from);
	void OnExecute(Player& cur);
	void OnExit(Player& cur, int to);
};

struct StatePlayerBlink
{
	void OnEnter(Player& cur, int from);
	void OnExecute(Player& cur);
	void OnExit(Player& cur, int to);
};

