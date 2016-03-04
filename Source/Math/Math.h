#include <iostream>

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