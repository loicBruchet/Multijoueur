#include "pch.h"
#define GAME_PROTOCOL_ID "STR_MRN_V1.0"

#define SPEED 45
App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);
}

App::~App()
{
}

void App::OnStart()
{

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET sckt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	m_pData = new ThreadData(); m_plData = new PlayerData(); m_netPacket = new NetPlayerOut();
	m_pData->sckt = sckt;
	m_pData->serverAddr.sin_family = AF_INET;
	m_pData->serverAddr.sin_port = htons(1888);
	m_font.Create(24/ WINDIVIDER);
	m_font2.Create(24 / WINDIVIDER, CPU_RED);
	m_spriteTexture.Load("..\\..\\res\\Textures\\menu.png");
	m_spriteTexture.height /= WINDIVIDER;
	m_spriteTexture.width /= WINDIVIDER;
	m_map = new Map;
	m_map->CreateMap();
	m_sprite = cpuEngine.CreateSprite();
	m_sprite->pTexture = &m_spriteTexture;
	m_sprite->x = (1920 / 2) / WINDIVIDER;
	m_sprite->y = (1080 / 2) / WINDIVIDER;
	m_sprite->CenterAnchor();
	m_meshPlayer.CreateSpaceship();
	m_meshMissile.CreateSphere(0.5f);
	m_materialPlayer.color = cpu::ToColor(255, 255, 5);
	m_materialMissile.color = cpu::ToColor(255, 255, 5);
	m_player = new Player;
	m_player->m_pSpawnPosition = m_map->m_pSpawnPosition;
	m_player->Create(&m_meshPlayer, &m_materialPlayer);
	m_player->GetFSM()->ToState(CPU_ID(StatePlayerSpawn));
	h1 = CreateThread(NULL, 0, SendMessCo, m_pData, 0, NULL);
	h2 = CreateThread(NULL, 0, SendInput, m_pData, 0, NULL);
	h3 = CreateThread(NULL, 0, ReceiveData, m_pData, 0, NULL);
	m_entities.push_back(m_player);

	cpuEngine.m_groundColor = cpu::ToColor(0, 0, 10);
	cpuEngine.m_skyColor = cpu::ToColor(6, 0, 20);
}

void App::OnUpdate()
{
	if (m_plData->validIP)
		m_sprite->pTexture = nullptr;

	float dt = cpuTime.delta;
	time += cpuTime.delta;

	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Player* pPlayer = *it;
		if (pPlayer->GetID() == m_plData->ID && m_plData->validIP)
		{
			m_control.Update(pPlayer, m_netPacket);
		}
	}

	if (cpuInput.IsKeyDown(VK_ESCAPE))
		cpuEngine.Quit();

	//Update missiles + death
	for (auto it = m_missiles.begin(); it != m_missiles.end(); ++it)
	{
		Projectile* pMissile = *it;
		pMissile->GetEntity()->transform.Move(dt * SPEED);
		if (pMissile->GetEntity()->lifetime > 10.0f)
			cpuEngine.Release(pMissile->GetEntity());
	}

	//remove dead missiles
	for (auto it = m_missiles.begin(); it != m_missiles.end(); )
	{
		if ((*it)->GetEntity()->dead)
			it = m_missiles.erase(it);
		else
			++it;
	}

	OnCollision();

	m_map->Update();
}

void App::OnExit()
{
	m_netPacket->type = PacketType::PLAYER_DISCONNECT;	
	writer.WriteDataClient(m_netPacket,m_pData, m_pData->sckt);

	m_isRunning = false;
	closesocket(m_pData->sckt);
	WaitForSingleObject(h1, INFINITE);
	WaitForSingleObject(h2, INFINITE);
	WaitForSingleObject(h3, INFINITE);
	CloseHandle(h1);
	CloseHandle(h2);
	CloseHandle(h3);
	delete m_plData;
	delete m_pData;
	delete m_netPacket;
	delete m_map;
	for (Projectile* pMissile : m_missiles)
	{
		delete pMissile;
		pMissile = nullptr;
	}
	delete m_player;
	m_entities.clear();	
	m_player = nullptr;
	m_netPacket = nullptr;
	m_plData = nullptr;
	m_pData = nullptr;
	m_map = nullptr;
	m_missiles.clear();

	WSACleanup();
}

void App::OnRender(int pass)
{
	switch (pass)
	{
		case CPU_PASS_PARTICLE_BEGIN:
		{
			// Blur particles
			//cpuEngine.SetRT(m_rts[0]);
			//cpuEngine.ClearColor();
			break;
		}
		case CPU_PASS_PARTICLE_END:
		{
			// Blur particles
			//cpuEngine.Blur(10);
			//cpuEngine.SetMainRT();
			//cpuEngine.AlphaBlend(m_rts[0]);
			break;
		}
		case CPU_PASS_UI_END:
		{
			// Debug
			/*cpu_stats& stats = *cpuEngine.GetStats();
			std::string info = CPU_STR(cpuTime.fps) + " fps, ";
			info += CPU_STR(stats.drawnTriangleCount) + " triangles, ";
			info += CPU_STR(stats.clipEntityCount) + " clipped entities\n";
			info += CPU_STR(stats.threadCount) + " threads, ";

			cpu_hit hit;
			for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
			{
				Player* pPlayer = *it;
				info += "\n Player: ";
				info += CPU_STR(pPlayer->GetEntity()->transform.pos.x).c_str();
				info += " / ";
				info += CPU_STR(pPlayer->GetEntity()->transform.pos.y).c_str();
				info += " / ";
				info += CPU_STR(pPlayer->GetEntity()->transform.pos.z).c_str();

			}*/
			for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
			{
				int count = 0;
				Player* pPlayer = *it;
				if (pPlayer->GetID() == m_plData->ID)
				{
					cpuDevice.DrawText(&m_font, CPU_STR(pPlayer->GetPoints()).c_str(), 0, 75 / WINDIVIDER, CPU_TEXT_LEFT);
					std::string coords("Coords : ");
					coords += CPU_STR((int)pPlayer->GetEntity()->transform.pos.x) + " / " + CPU_STR((int)pPlayer->GetEntity()->transform.pos.y) + " / " + CPU_STR((int)pPlayer->GetEntity()->transform.pos.z);
					cpuDevice.DrawText(&m_font, coords.c_str(), 0, 100 / WINDIVIDER, CPU_TEXT_LEFT);
					std::string healthstr("Health : ");
					healthstr += CPU_STR(pPlayer->GetHealth());
					cpuDevice.DrawText(&m_font, healthstr.c_str(), 0, 125 / WINDIVIDER, CPU_TEXT_LEFT);
				}
				else
				{
					std::string playerData("Player : ");
					playerData += CPU_STR(pPlayer->GetID());
					playerData += " - HP : ";
					playerData += CPU_STR(pPlayer->GetHealth());
					cpuDevice.DrawText(&m_font, playerData.c_str(), 0, (150 + (25 * count)) / WINDIVIDER, CPU_TEXT_LEFT);
					count++;
				}
			}
			std::string keystats("Keyboard : ");
			keystats += (TrdValue.Layout == Keyboardlayout::AZERTY) ? "AZERTY (P)" : "QWERTY (P)";
			cpu_font f = m_font;
			if (!IPDone && !m_plData->validIP)
			{
				cpuDevice.DrawText(&m_font2, m_plData->ServerIP.c_str(), 520 / WINDIVIDER, 400 / WINDIVIDER, CPU_TEXT_LEFT);
			}
			else if (IPDone && !m_plData->validIP)
			{
				cpuDevice.DrawText(&m_font, m_plData->ServerIP.c_str(), 520 / WINDIVIDER, 400 / WINDIVIDER, CPU_TEXT_LEFT);
				cpuDevice.DrawText(&m_font2, m_plData->playerName.c_str(), 520 / WINDIVIDER, 535 / WINDIVIDER, CPU_TEXT_LEFT);
			}
			else if (IPDone && m_plData->validIP)
			{
				cpuDevice.DrawText(&m_font, m_plData->ServerIP.c_str(), 0, 0, CPU_TEXT_LEFT);
				cpuDevice.DrawText(&m_font2, m_plData->playerName.c_str(), 0, 25 / WINDIVIDER, CPU_TEXT_LEFT);
			}
			cpuDevice.DrawText(&m_font, keystats.c_str(), 0, 50 / WINDIVIDER, CPU_TEXT_LEFT);
			break;
		}
	}

}

void App::SpawnMissile(XMFLOAT3 pos, XMFLOAT3 dir, Player* pl)
{
	Projectile* pProjectile = new Projectile();
	pProjectile->Create(&m_meshMissile, &m_materialMissile, pos, dir);
	pProjectile->m_pPlayerEntityUser = pl->GetEntity();
	m_missiles.push_back(pProjectile);

}

void App::OnCollision()
{
	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Player* pPlayer = *it;
		for (auto ito = m_missiles.begin(); ito != m_missiles.end(); ++ito)
		{
			int a= 0;
			Projectile* pProjectile = *ito;
			if (pPlayer->GetEntity()->aabb.Contains(pProjectile->GetEntity()->transform.pos))
			{
				a++;
				if (pPlayer->GetState() !=2)
				{
					pPlayer->SetHealth(pPlayer->GetHealth() - 10);
					for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
					{
						Player* m_player = *it;
						if (m_player->GetID() == m_plData->ID)
						{
							if (pPlayer->GetHealth() <= 0 && pProjectile->m_pPlayerEntityUser == m_player->GetEntity())
							{
								m_player->SetPoints(pPlayer->GetPoints() + 10);
							}
						}
						pPlayer->GetFSM()->ToState(CPU_ID(StatePlayerBlink));
					}
				}
				cpuEngine.Release(pProjectile->GetEntity());
				break;
			}
		}


		for (auto mapit = m_map->entitiesMap.begin(); mapit != m_map->entitiesMap.end(); ++mapit)
		{
			cpu_entity* pEntityMap = *mapit;
			for (auto ito = m_missiles.begin(); ito != m_missiles.end(); ++ito)
			{
				Projectile* pProjectile = *ito;
				if (pEntityMap->aabb.Contains(pProjectile->GetEntity()->transform.pos))
				{
					cpuEngine.Release(pProjectile->GetEntity());
				}
			}
		}
	}
}

DWORD WINAPI App::SendMessCo(LPVOID lpParam) {
	ThreadData* datas = static_cast<ThreadData*>(lpParam);
	bool NameDone = false;
	bool keyState[256] = { false };
	App::GetInstance().IPDone = false;
	while (App::GetInstance().IPDone == false && App::GetInstance().m_isRunning)
	{
		for (int i = 8; i <= 255; i++) {
			if ((GetAsyncKeyState(i) & 0x8000) && !keyState[i]) {
				keyState[i] = true;
				if (i == VK_RETURN && !App::GetInstance().m_plData->ServerIP.empty()) {
					App::GetInstance().IPDone = true;
				}
				else if (i == VK_BACK && !App::GetInstance().m_plData->ServerIP.empty()) {
					App::GetInstance().m_plData->ServerIP.pop_back();
				}
				else {
					char c = (char)MapVirtualKey(i, MAPVK_VK_TO_CHAR);
					if (c >= 46 && c <= 57 && !App::GetInstance().IPDone)
						App::GetInstance().m_plData->ServerIP += c;
				}
			}
			else if (!(GetAsyncKeyState(i) & 0x8000)) {
				keyState[i] = false;
			}
		}
		Sleep(10);
	}

	inet_pton(AF_INET, App::GetInstance().m_plData->ServerIP.c_str(), (char*)&datas->serverAddr.sin_addr);
	while (App::GetInstance().m_isRunning && !App::GetInstance().m_plData->validIP) {
		for (int i = 8; i <= 255; i++) {
			if ((GetAsyncKeyState(i) & 0x8000) && !keyState[i]) {
				keyState[i] = true;
				if (i == VK_RETURN && !App::GetInstance().m_plData->playerName.empty()) {
					NameDone = true;
					std::string request = std::string(GAME_PROTOCOL_ID) + ":" + App::GetInstance().m_plData->playerName;
					sendto(datas->sckt, request.c_str(), (int)request.length(), 0, (sockaddr*)&datas->serverAddr, sizeof(datas->serverAddr));
				}
				else if (i == VK_BACK && !App::GetInstance().m_plData->playerName.empty()) {
					App::GetInstance().m_plData->playerName.pop_back();
				}
				else {
					char c = (char)MapVirtualKey(i, MAPVK_VK_TO_CHAR);
					if (c >= 32 && !NameDone) App::GetInstance().m_plData->playerName += c;
				}
			}
			else if (!(GetAsyncKeyState(i) & 0x8000)) {
				keyState[i] = false;
			}
		}
		Sleep(10);
	}
	return 0;
}

DWORD WINAPI App::SendInput(LPVOID lpParam)
{
	ThreadData* datas = static_cast<ThreadData*>(lpParam);
	std::string input;					
	App::GetInstance().m_netPacket->type = PacketType::PLAYER_MOVEMENT;
	while (!datas->connected && App::GetInstance().m_isRunning)
	{
		Sleep(10);
	}
	while (App::GetInstance().m_isRunning) {
		App::GetInstance().TrdValue.layoutChangeCooldown += cpuTime.delta;
		for (int i = 8; i <= 255; i++) {
			if (GetAsyncKeyState(i) & 0x8000) {
				char c = (char)MapVirtualKey(i, MAPVK_VK_TO_CHAR);
				App::GetInstance().m_netPacket->action = App::GetInstance().TrdValue.GetInputAction(c);
				if (cpuInput.IsKey(VK_LCONTROL))
					App::GetInstance().m_netPacket->action = PlayerActions::ACTION_DOWN;
				App::GetInstance().m_netPacket->life = App::GetInstance().m_player->GetHealth();
				App::GetInstance().m_control.Moved(App::GetInstance().writer, App::GetInstance().m_netPacket, datas, App::GetInstance().m_pData->sckt);
				input.clear();
			}
			else if (cpuInput.IsKey(VK_LBUTTON) && App::GetInstance().time > 0.2f && App::GetInstance().m_entities[App::GetInstance().m_plData->ID]->GetState() != 2)
			{
				App::GetInstance().m_netPacket->action = PlayerActions::ACTION_SHOOT;
				App::GetInstance().m_netPacket->life = App::GetInstance().m_player->GetHealth();
				App::GetInstance().m_control.Moved(App::GetInstance().writer, App::GetInstance().m_netPacket, datas, App::GetInstance().m_pData->sckt);
				App::GetInstance().time = 0;
			}
			else if (App::GetInstance().m_player->GetHealth() == 0)
			{
				App::GetInstance().m_netPacket->action = PlayerActions::ACTION_NONE;
				App::GetInstance().m_netPacket->life = App::GetInstance().m_player->GetHealth();
				App::GetInstance().m_control.Moved(App::GetInstance().writer, App::GetInstance().m_netPacket, datas, App::GetInstance().m_pData->sckt);
			}

		}
		Sleep(10);
	}
	return 0;
}

DWORD WINAPI App::ReceiveData(LPVOID lpParam) {
	ThreadData* datas = static_cast<ThreadData*>(lpParam);
	char buffer[1500];
	sockaddr_in fromAddr;
	socklen_t fromLen = sizeof(fromAddr);

	//wait to be connected
	while (App::GetInstance().m_isRunning && !App::GetInstance().m_plData->validIP) {
		int ret = recvfrom(datas->sckt, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&fromAddr, &fromLen);

		if (ret > 0) {
			buffer[ret] = '\0';
			std::string msg(buffer);
			std::string expectedHeader = std::string(GAME_PROTOCOL_ID) + "_AUTH_OK:";
			std::string expectedFull = expectedHeader + App::GetInstance().m_plData->playerName;
			if (msg.find(expectedFull) != std::string::npos)
			{
				App::GetInstance().m_plData->validIP = true;
				datas->connected = true;			
				App::GetInstance().m_plData->ID = std::stoi(msg.substr(expectedFull.size()));
				App::GetInstance().m_netPacket->id = App::GetInstance().m_plData->ID;
				for (auto it = App::GetInstance().m_entities.begin(); it != App::GetInstance().m_entities.end(); ++it)
				{
					Player* pPlayer = *it;
					pPlayer->SetID(App::GetInstance().m_plData->ID);
					pPlayer->SetHealth(100);
				}
			}
		}
	}

	//receive data loop for the game
	while (App::GetInstance().m_isRunning) {
		int ret = recvfrom(datas->sckt, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&fromAddr, &fromLen);
		if (ret > 0) {
			buffer[ret] = '\0';
			NetPlayerOut* aPDta = (NetPlayerOut*)buffer;
			bool found = false;
			bool disconnect = (aPDta->type == PacketType::PLAYER_DISCONNECT);
			for (auto it = App::GetInstance().m_entities.begin(); it != App::GetInstance().m_entities.end(); ++it)
			{
				Player* pPlayer = *it;
				if (pPlayer->GetID() == aPDta->id)
				{
					pPlayer->GetEntity()->transform = App::GetInstance().TrdValue.TradeFloatPacketToTransform(aPDta, pPlayer->GetEntity()->transform);
					float dx = aPDta->directionX;
					float dy = aPDta->directionY;
					float dz = aPDta->directionZ;
					float pitch = asin(-dy);
					float yaw = atan2(dx, dz);
					pPlayer->GetEntity()->transform.SetYPR(yaw, pitch, 0);
					pPlayer->SetHealth(aPDta->life);
					if (aPDta->revive)
					{
						pPlayer->Revive();
					}
					found = true;
					if (aPDta->action == PlayerActions::ACTION_SHOOT)
						App::GetInstance().SpawnMissile(pPlayer->GetEntity()->transform.pos, pPlayer->GetEntity()->transform.dir, pPlayer);
				}
			}
			if (!found)
			{
				Player* newPlayer = new Player;
				newPlayer->SetID(aPDta->id);
				newPlayer->Create(&App::GetInstance().m_meshPlayer, &App::GetInstance().m_materialPlayer);
				newPlayer->GetEntity()->transform.pos.x = aPDta->posx;
				newPlayer->GetEntity()->transform.pos.y = aPDta->posy;
				newPlayer->GetEntity()->transform.pos.z = aPDta->posz;
				newPlayer->SetHealth(aPDta->life);
				App::GetInstance().m_entities.push_back(newPlayer);
			}
			if (disconnect)
			{
				for (auto it = App::GetInstance().m_entities.begin(); it != App::GetInstance().m_entities.end(); ++it)
				{
					Player* pPlayer = *it;
					if (pPlayer->GetID() == aPDta->id)
					{
						cpuEngine.Release(pPlayer->GetEntity());
						App::GetInstance().m_entities.erase(it);
						break;
					}
				}
			}
		}
	}
	return 0;
}

void App::MissileShader(cpu_ps_io& io)
{
	// garder seulement le rouge du pixel éclairé
	io.color.x = io.p.color.x;
}