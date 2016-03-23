#include "Object3D.h"
#ifndef __WORLD_H__
#define __WORLD_H__

class World : public Object3D
{
public:
	virtual void Start();
	virtual void Update( float dt );
	virtual void Draw();

	Vector3f GetWorldPosition( Vector2i pos );

private:
	Vector3f mWorldPos[8][8];
	float mTileHalfSize;
};
#endif