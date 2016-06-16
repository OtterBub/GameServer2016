#include "Math.h"

bool RadiusCheck(Vector2i apos, Vector2i bpos, float Radius)
{
	float dist =
		(apos.x - bpos.x) * (apos.x - bpos.x)
		+
		(apos.y - bpos.y) * (apos.y - bpos.y);

	return (dist <= (Radius * Radius));
}

bool SquareCheck(Vector2i pos, Vector2i target, float halfsize)
{
	bool result =
		(pos.x + halfsize >= target.x) &&
		(pos.x - halfsize <= target.x) &&
		(pos.y + halfsize >= target.y) &&
		(pos.y - halfsize <= target.y);

	return result;
}

bool SquareCheck(Vector2f pos, Vector2f target, float halfsize)
{
	bool result =
		(pos.x + halfsize >= target.x) &&
		(pos.x - halfsize <= target.x) &&
		(pos.y + halfsize >= target.y) &&
		(pos.y - halfsize <= target.y);

	return result;
}