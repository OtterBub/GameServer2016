#include <iostream>

#ifndef __MATH_H__
#define __MATH_H__

template <typename T>
class Vector2
{
public:
	Vector2()
	{
		x = y = 0;
	}
	Vector2(T x_, T y_)
	{
		this->x = x_;
		this->y = y_;
	}
	T x, y;
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;

template <typename T>
class Vector3
{
public:
	Vector3()
	{
		x = y = z = 0;
	}
	Vector3(T x_, T y_, T z_)
	{
		this->x = x_;
		this->y = y_;
		this->z = z_;
	}
	T x, y, z;
};

typedef Vector3<float> Vector3f;

bool RadiusCheck(Vector2i apos, Vector2i bpos, float Radius);
bool SquareCheck(Vector2i pos, Vector2i target, float halfsize);
bool SquareCheck(Vector2f pos, Vector2f target, float halfsize);

#endif 