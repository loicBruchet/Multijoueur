#include "pch.h"
#include <iostream>
#include <ws2tcpip.h>
#include <set>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <random>

#pragma comment(lib, "ws2_32.lib")

#define GAME_PROTOCOL_ID "STR_MRN_V1.0"

struct AllPlayersData {
    std::map<uint32_t, Player> players;
    std::map<uint32_t, cpu_entity> entities;
};

AllPlayersData aPDta;
cpu_transform checkPlayersCoord(cpu_transform temp)
{
    if (temp.pos.x > 50)
    {
        temp.pos.x -= 1;
    }
    if (temp.pos.x < -50)
    {
        temp.pos.x += 1;
    }
    if (temp.pos.y > 50)
    {
        temp.pos.y -= 1;
    }
    if (temp.pos.y < -50)
    {
        temp.pos.y += 1;
    }
    if (temp.pos.z > 50)
    {
        temp.pos.z -= 1;
    }
    if (temp.pos.z < -50)
    {
        temp.pos.z += 1;
    }
	return temp;
}

void ActionPlayer(cpu_transform& ent, uint32_t action)
{
	cpu_transform& temp = ent;
	float spd = 0.3f;
    switch (action)
    {
	case ACTION_FORWARD: temp.Move(spd); ent = checkPlayersCoord(temp); break;
	case ACTION_BACKWARD: temp.Move(-spd); ent = checkPlayersCoord(temp); break;
	case ACTION_LEFT: temp.MoveSide(-spd); ent = checkPlayersCoord(temp); break;
	case ACTION_RIGHT: temp.MoveSide(spd); ent = checkPlayersCoord(temp); break;
	case ACTION_UP: temp.MoveUp(spd); ent = checkPlayersCoord(temp); break;
	case ACTION_DOWN: temp.MoveUp(-spd/2); ent = checkPlayersCoord(temp); break;
    default:			break;
    }
}

int main()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
    SOCKET sckt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    std::srand(time(NULL));

    int optval = 1;
    setsockopt(sckt, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));

    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(1888);

    if (bind(sckt, (sockaddr*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
        return 3;
    }

    std::cout << "Server running on port 1888..." << std::endl;

    char buffr[1500];
    std::set<sockaddr_in, sockaddr_cmp> clients;
    std::vector<ClientInfo> c;
    int clcount = 0;

    while (true) {
        sockaddr_in from;
        int fromlen = sizeof(from);
        int rez = recvfrom(sckt, buffr, 1499, 0, (sockaddr*)&from, &fromlen);

        if (rez > 0) {
            buffr[rez] = '\0';
            bool found = (clients.find(from) != clients.end());

            if (!found && std::string(buffr).find(std::string(GAME_PROTOCOL_ID) + ":") != std::string::npos) {
                std::string prefix = std::string(GAME_PROTOCOL_ID) + ":";
                std::string clientName = std::string(buffr).substr(prefix.size());

                ClientInfo ci;
                ci.name = clientName;
                ci.ID = clcount;
                ci.addr = from;
                c.push_back(ci);
                clients.insert(from);

                Player newP;
                newP.SetID(ci.ID);
                aPDta.players[clcount] = newP;
                aPDta.entities[clcount] = cpu_entity();
                int px = ((std::rand() % 2) == 1 ? 1 : -1);
                int py = ((std::rand() % 2) == 1 ? 1 : -1);
                int pz = ((std::rand() % 2) == 1 ? 1 : -1);
                aPDta.entities[clcount].transform.SetPosition((std::rand() % 45)*px, (std::rand() % 45) * py, (std::rand() % 45) * pz);

                std::cout << "New client: " << ci.name << " ID: " << clcount << std::endl;

                std::string sendMsg = std::string(GAME_PROTOCOL_ID) + "_AUTH_OK:" + ci.name + std::to_string(ci.ID);
                sendto(sckt, sendMsg.c_str(), (int)sendMsg.size(), 0, (sockaddr*)&from, sizeof(from));

                clcount++;
            }
            else if (found)
            {
                if (rez >= sizeof(NetPlayerOut)) {
                    NetPlayerOut* packetIn = (NetPlayerOut*)buffr;
                    TradeValue TrdValue;
                    bool disconnect = (packetIn->type == PLAYER_DISCONNECT);

                    if (packetIn->life > 0)
                    {
                        packetIn->posx = aPDta.entities[packetIn->id].transform.pos.x;
                        packetIn->posy = aPDta.entities[packetIn->id].transform.pos.y;
                        packetIn->posz = aPDta.entities[packetIn->id].transform.pos.z;
                        aPDta.entities[packetIn->id].transform = TrdValue.TradeFloatPacketToTransform(packetIn, aPDta.entities[packetIn->id].transform);

                        if (aPDta.players.find(packetIn->id) != aPDta.players.end()) {
                            if (packetIn->type == PLAYER_MOVEMENT) {
                                ActionPlayer(aPDta.entities[packetIn->id].transform, packetIn->action);
                            }
                        }
                        int i = packetIn->type;
                        packetIn->posx = aPDta.entities[packetIn->id].transform.pos.x;
                        packetIn->posy = aPDta.entities[packetIn->id].transform.pos.y;
                        packetIn->posz = aPDta.entities[packetIn->id].transform.pos.z;
                        packetIn->revive = false;
                    }
                    else 
                    {
                        int px = ((std::rand() % 2) == 1 ? 1 : -1);
                        int py = ((std::rand() % 2) == 1 ? 1 : -1);
                        int pz = ((std::rand() % 2) == 1 ? 1 : -1);
                        aPDta.entities[packetIn->id].transform.SetPosition((std::rand() % 45) * px, (std::rand() % 45) * py, (std::rand() % 45) * pz);
						packetIn->posx = aPDta.entities[packetIn->id].transform.pos.x;
						packetIn->posy = aPDta.entities[packetIn->id].transform.pos.y;
						packetIn->posz = aPDta.entities[packetIn->id].transform.pos.z;
                        packetIn->life = 100;
                        packetIn->revive = true;
                    }

                    PacketsWriter writer;
                    int playerCount = (int)aPDta.players.size();
					if (playerCount > 0 && !disconnect)
                        writer.WriteDataServer(packetIn, c, sckt);
                    if (disconnect) {
                        aPDta.players.erase(packetIn->id);
                        aPDta.entities.erase(packetIn->id);
                        for (auto it = c.begin(); it != c.end(); ++it) {
                            if (it->ID == packetIn->id) {
                                std::cout << "Client ID " << packetIn->id << ": " << it->name << " disconnected." << std::endl;
                                c.erase(it);
                                writer.WriteDataServer(packetIn, c, sckt);
                                break;
                            }
						}
                        clients.erase(from);
                    }
                }
            }
        }
    }
    closesocket(sckt);
    WSACleanup();
    return 0;
}