#pragma once
#include "framework.h"

class PacketsWriter
{
public:

	void WriteDataServer(NetPlayerOut* nPO, std::vector<ClientInfo> c, SOCKET sckt);
	void WriteDataClient(NetPlayerOut* nPO, ThreadData* server, SOCKET sckt);

};