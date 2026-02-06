#pragma once
struct ThreadData
{
	SOCKET sckt;
	sockaddr_in serverAddr;
	bool connected = false;
};

struct PlayerData
{
	std::string playerName;
	std::string ServerIP;
	bool validIP = false;
	uint32_t ID = 0;
};

enum PacketType : uint8_t
{
	PLAYER_CONNECT = 0,
	PLAYER_MOVEMENT = 1,
	PLAYER_ACTION = 2,
	PLAYER_DISCONNECT = 3
};

enum PlayerActions
{
	ACTION_NONE = 0,
	ACTION_FORWARD = 1,
	ACTION_BACKWARD = 2,
	ACTION_LEFT = 3,
	ACTION_RIGHT = 4,
	ACTION_UP = 5,
	ACTION_DOWN = 6,
	ACTION_SHOOT = 7
};

struct sockaddr_cmp {
	bool operator()(const sockaddr_in& lhs, const sockaddr_in& rhs) const {
		if (lhs.sin_addr.s_addr != rhs.sin_addr.s_addr)
			return lhs.sin_addr.s_addr < rhs.sin_addr.s_addr;
		return lhs.sin_port < rhs.sin_port;
	}
};

struct ClientInfo {
	std::string name;
	sockaddr_in addr;
	uint32_t ID;
};

enum Keyboardlayout
{
	AZERTY,
	QWERTY
};
#pragma pack(push, 1)
struct NetPlayerOut {
	uint32_t id;
	uint32_t type;
	uint32_t action;
	float posx;
	float posy;
	float posz;
	float directionX;
	float directionY;
	float directionZ;
	float rightX;
	float rightY;
	float rightZ;
	float upX;
	float upY;
	float upZ;
	int life;
	bool revive;
};
#pragma pack(pop)