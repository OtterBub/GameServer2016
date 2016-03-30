#ifndef PACKETSTRUCT_H
#define PACKETSTRUCT_H

#define TYPE_LOGIN 1
#define TYPE_WORLD 2

enum moveDir {
	MOVE_RIGHT = 0x0001,
	MOVE_LEFT = 0x0002,
	MOVE_UP = 0x0004 ,
	MOVE_DOWN = 0x0008
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

#endif 