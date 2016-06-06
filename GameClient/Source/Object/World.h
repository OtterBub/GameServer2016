#ifndef __WORLD_H__
#define __WORLD_H__
#include "Object3D.h"

#define WORLDSIZE_X 100
#define WORLDSIZE_Y 100

class World : public Object3D
{
public:
	virtual void Start();
	virtual void Update( double dt );
	virtual void Draw();
	void Draw(Vector2i pos, int ViewRange);

	Vector3f GetWorldPosition( Vector2i pos );

private:
	Vector3f mWorldPos[WORLDSIZE_X][WORLDSIZE_Y];
	float mTileHalfSize;
};
#endif