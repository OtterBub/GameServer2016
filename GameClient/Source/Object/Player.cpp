#include "Player.h"

Player::Player()
{
	mPosition.x = 0;
	mPosition.y = 0;
	mPosition.z = 0;
	mWorldPos.x = 4;
	mWorldPos.y = 4;
}

void Player::Start()
{
	mWorldPos.x = 4;
	mWorldPos.y = 4;
}
void Player::Update( float dt )
{}
void Player::Draw()
{
	glPushMatrix();
	{
		mPosition.y = 9;
		glTranslatef( mPosition.x, mPosition.y, mPosition.z );
		glColor4f( 1, 1, 1, 1 );
		glutSolidCube( 10 );
		glColor4f( 0, 0, 0, 0 );
		glutWireCube( 10.1 );

		float shadowHalf = 10 / 2.f;
		glColor4f( 0, 0, 0, 1.0 );
		glBegin( GL_QUADS );
		glVertex3f( shadowHalf, -mPosition.y + 0.5, -shadowHalf );
		glVertex3f( -shadowHalf, -mPosition.y + 0.5, -shadowHalf );
		glVertex3f( -shadowHalf, -mPosition.y + 0.5, shadowHalf );
		glVertex3f( shadowHalf, -mPosition.y + 0.5, shadowHalf );
		glEnd();
	}
	glPopMatrix();
}

void Player::SetPosition( Vector3f pos )
{
	mPosition = pos;
}

void Player::SetWorldposition( Vector2i pos )
{
	mWorldPos = pos;
}

Vector2i Player::GetWorldPos()
{
	return mWorldPos;
}