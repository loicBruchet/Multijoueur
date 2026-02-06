#include "pch.h"

void PacketsWriter::WriteDataServer(NetPlayerOut* nPO, std::vector<ClientInfo> c, SOCKET sckt)
{
	char sendBuffer[4096];
	size_t offset = 0;
	auto write = [&](const void* data, size_t size) {
		memcpy(sendBuffer + offset, data, size);
		offset += size;
		};
	write(&nPO->id, sizeof(nPO->id));
	write(&nPO->type, sizeof(nPO->type));
	write(&nPO->action, sizeof(nPO->action));
	write(&nPO->posx, sizeof(nPO->posx));
	write(&nPO->posy, sizeof(nPO->posy));
	write(&nPO->posz, sizeof(nPO->posz));
	write(&nPO->directionX, sizeof(nPO->directionX));
	write(&nPO->directionY, sizeof(nPO->directionY));
	write(&nPO->directionZ, sizeof(nPO->directionZ));
	write(&nPO->rightX, sizeof(nPO->rightX));
	write(&nPO->rightY, sizeof(nPO->rightY));
	write(&nPO->rightZ, sizeof(nPO->rightZ));
	write(&nPO->upX, sizeof(nPO->upX));
	write(&nPO->upY, sizeof(nPO->upY));
	write(&nPO->upZ, sizeof(nPO->upZ));
	write(&nPO->life, sizeof(nPO->life));
	write(&nPO->revive, sizeof(nPO->revive));
	for (auto& client : c) {
		sendto(sckt, sendBuffer, offset, 0, (sockaddr*)&client.addr, sizeof(client.addr));
	}
}

void PacketsWriter::WriteDataClient(NetPlayerOut* nPO, ThreadData* server, SOCKET sckt)
{
	char sendBuffer[4096] = { "0" };
	size_t offset = 0;
	auto write = [&](const void* data, size_t size) {
		memcpy(sendBuffer + offset, data, size);
		offset += size;
		};

	write(&nPO->id, sizeof(nPO->id));
	write(&nPO->type, sizeof(nPO->type));
	write(&nPO->action, sizeof(nPO->action));
	write(&nPO->posx, sizeof(nPO->posx));
	write(&nPO->posy, sizeof(nPO->posy));
	write(&nPO->posz, sizeof(nPO->posz));
	write(&nPO->directionX, sizeof(nPO->directionX));
	write(&nPO->directionY, sizeof(nPO->directionY));
	write(&nPO->directionZ, sizeof(nPO->directionZ));
	write(&nPO->rightX, sizeof(nPO->rightX));
	write(&nPO->rightY, sizeof(nPO->rightY));
	write(&nPO->rightZ, sizeof(nPO->rightZ));
	write(&nPO->upX, sizeof(nPO->upX));
	write(&nPO->upY, sizeof(nPO->upY));
	write(&nPO->upZ, sizeof(nPO->upZ));
	write(&nPO->life, sizeof(nPO->life));
	write(&nPO->revive, sizeof(nPO->revive));
	sendto(sckt, sendBuffer, offset, 0, (sockaddr*)&server->serverAddr, sizeof(server->serverAddr));
}