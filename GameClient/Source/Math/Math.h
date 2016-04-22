#include <iostream>

#ifndef __MATH_H__
#define __MATH_H__

template <typename T>
class Vector2
{
public:
	T x, y;
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;

template <typename T>
class Vector3
{
public:
	T x, y, z;
};

typedef Vector3<float> Vector3f;

bool RadiusCheck(Vector2i apos, Vector2i bpos, float Radius);
bool SquareCheck(Vector2i pos, Vector2i target, float halfsize);

#endif 