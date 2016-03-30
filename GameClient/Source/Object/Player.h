#include "Object3D.h"
#ifndef __PLAYER_H__
#define __PLAYER_H__

class Player : public Object3D
{
public:
	Player();

	virtual void Start();
	virtual void Update( float dt );
	virtual void Draw();
	void Draw( int x, int y );

	void SetPosition( Vector3f pos );
	void SetWorldposition( Vector2i pos );
	void SetPlayerID( int id );
	Vector2i GetWorldPos();

private:
	Vector3f mPosition;
	Vector2i mWorldPos;

	int mID;
};
#endif