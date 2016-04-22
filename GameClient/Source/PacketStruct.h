#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

#define CS_TYPE_MOVE 1

#pragma pack (push, 1)

enum moveDir {
	MOVE_RIGHT = 0x0001,
	MOVE_LEFT = 0x0002,
	MOVE_UP = 0x0004,
	MOVE_DOWN = 0x0008
};

struct packet_header
{
	BYTE size;
	BYTE type;
};

// client to server
struct cs_packet_move
{
	packet_header header;
	unsigned int moveDir;
};

// server to client
struct sc_packet_plsyer_pos
{
	packet_header header;
	int x, y;
};



struct playerPos
{
	int x, y;
};

struct playerInfo
{
	int Id;
	bool login;
	playerPos pos;
};

struct WorldData {
	char type;
	playerInfo playerInfo[20];
};

#pragma pack (pop)

#endif 