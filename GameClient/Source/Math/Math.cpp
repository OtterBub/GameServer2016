#include "Math.h"

bool RadiusCheck(Vector2i apos, Vector2i bpos, float Radius)
{
	float dist =
		(apos.x - bpos.x) * (apos.x - bpos.x)
		+
		(apos.y - bpos.y) * (apos.y - bpos.y);

	return (dist <= (Radius * Radius));
}