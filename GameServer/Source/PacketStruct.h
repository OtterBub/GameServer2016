#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

// client to server
#define CS_TYPE_MOVE 1

// server to client
#define SC_TYPE_MOVE 1

#define MAX_VIEW_USER 20
#define MAX_BUFF_SIZE 1024
#define MAX_PACKET_SIZE 255
#define SERVER_PORT 4000
#define MAX_USER 1000

#define OP_RECV 1
#define OP_SEND 2


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
	packet_header()
	{
		size = 0;
		type = 0;
	}
};

// client to server
struct cs_packet_move
{
	packet_header header;
	unsigned int moveDir;
};

// server to client
struct sc_packet_player_pos
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
	bool view;
	playerPos pos;
};

struct WorldData {
	char type;
	playerInfo playerInfo[MAX_VIEW_USER];
};

#pragma pack (pop)

#endif 