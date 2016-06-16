#pragma once
#include "Object.h"


class ObjectWorld : public Object
{
public:
	ObjectWorld();
	~ObjectWorld();
	void Draw();
	void Update(double dt);
	void Reshape(int w, int h);
private:

};