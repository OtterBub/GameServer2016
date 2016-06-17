#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

// client to server
#define CS_LOGIN 1
#define CS_MOVE 2
#define CS_ATTACK 3
#define CS_CHAT 4
#define CS_LOGOUT 5

#define CS_TYPE_MOVE 1
#define CS_TYPE_PLAYER_POS 2

// server to client
#define SC_LOGIN_OK 1
#define SC_LOGIN_FAIL 2
#define SC_POSITION_INFO 3
#define SC_CHAT 4

#define SC_REMOVE_OBJECT 6
#define SC_ADD_OBJECT 7

// add object type
#define TYPE_PLAYER 1
#define TYPE_MONSTER 2
#define TYPE_MONSTER2 3

#define SC_TYPE_MOVE 101
#define SC_TYPE_PLAYER_REMOVE 102

#define MAX_VIEW_USER 20
#define MAX_BUFF_SIZE 1024
#define MAX_PACKET_SIZE 255
#define SERVER_PORT 4000
#define MAX_USER 5000000

#define OP_RECV 1
#define OP_SEND 2
#define OP_MOVE_AI 3

#define OP_TEST 100


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
struct cs_packet_login
{
	packet_header header;
	WCHAR nick[10];

	cs_packet_login()
	{
		memset(this, 0, sizeof(cs_packet_login));
		header.size = sizeof(cs_packet_login);
		header.type = CS_LOGIN;
	}
};

struct cs_packet_move
{
	packet_header header;
	BYTE moveDir;
	cs_packet_move()
	{
		memset(this, 0, sizeof(cs_packet_move));
		header.size = sizeof(cs_packet_move);
		header.type = CS_MOVE;
	}
};

struct cs_packet_player_pos
{
	packet_header header;	
	float x, y;
};

// server to client
struct sc_packet_login_ok
{
	packet_header header;
	WORD id;
	WORD x_pos;
	WORD y_pos;
	WORD HP;
	WORD LEVEL;
	WORD EXP;
	sc_packet_login_ok()
	{
		memset(this, 0, sizeof(sc_packet_login_ok));
		this->header.size = sizeof(sc_packet_login_ok);
		this->header.type = SC_LOGIN_OK;
	}
};

struct sc_packet_position_info
{
	packet_header header;
	WORD id;
	WORD x_pos;
	WORD y_pos;
	BYTE objType; //add

	sc_packet_position_info()
	{
		memset(this, 0, sizeof(sc_packet_position_info));
		this->header.size = sizeof(sc_packet_position_info);
		this->header.type = SC_POSITION_INFO;
	}
};

struct sc_packet_add_object
{
	packet_header header;
	WORD id;
	WORD x_pos; // add
	WORD y_pos; // add
	BYTE objType;
	sc_packet_add_object()
	{
		memset(this, 0, sizeof(sc_packet_add_object));
		this->header.size = sizeof(sc_packet_add_object);
		this->header.type = SC_ADD_OBJECT;
	}
};

struct sc_packet_remove_object
{
	packet_header header;
	WORD id;
	BYTE objType;
	sc_packet_remove_object()
	{
		memset(this, 0, sizeof(sc_packet_remove_object));
		this->header.size = sizeof(sc_packet_remove_object);
		this->header.type = SC_REMOVE_OBJECT;
	}
};

struct sc_packet_player_pos
{
	packet_header header;
	int id;
	float x, y;
};

struct sc_packet_player_remove
{
	packet_header header;
	int id;
};

struct playerPos
{
	float x, y;
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