#include "..//Common.h"
#ifndef __OBJECT_H__
#define __OBJECT_H__

class Object
{
public:
	virtual void Start() = 0;
	virtual void Update( float dt ) = 0;
	virtual void Draw() = 0;

private:

};

#endif