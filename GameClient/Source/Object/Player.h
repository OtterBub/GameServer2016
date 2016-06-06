#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Object3D.h"

class Player : public Object3D
{
public:
	Player();

	virtual void Start();
	virtual void Update( double dt );
	virtual void Draw();
	void Draw( int x, int y );

	void SetPosition( Vector3f pos );
	void SetWorldposition( Vector2i pos );
	void SetPlayerID( int id );
	Vector2i GetWorldPos();
	Vector3f GetPlayerPos();

private:
	Vector3f mPosition;
	Vector2i mWorldPos;

	int mID;
};
#endif