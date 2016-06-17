#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "..\Common.h"
#include "..\Math\Math.h"

class Player
{
public:
	int id;
	wchar_t char_id[200];
	Vector3<int> mPos;
	int exp;
	int atk;
	int def;
	int hp;
	int level;

	Player()
	{
		memset(this, 0, sizeof(Player));
	}
private:

};

#endif