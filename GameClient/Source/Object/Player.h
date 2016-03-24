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

	void SetPosition( Vector3f pos );
	void SetWorldposition( Vector2i pos );
	Vector2i GetWorldPos();

private:
	Vector3f mPosition;
	Vector2i mWorldPos;
};
#endif