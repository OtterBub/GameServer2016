#include "World.h"

void World::Start()
{
	float WorldSize = 160;
	float TileSize = WorldSize / 8;
	mTileHalfSize = TileSize / 2.f;

	for(int i = 0; i < WORLDSIZE_Y; i++)
	{
		for(int j = 0; j < WORLDSIZE_X; j++)
		{
			mWorldPos[j][i].x = TileSize * j;
			mWorldPos[j][i].z = TileSize * i;
			mWorldPos[j][i].y = 0;
		}
	}
}
void World::Update( float dt )
{

}
void World::Draw()
{
	glPushMatrix();
	for(int i = 0; i < WORLDSIZE_Y; i++)
	{
		for(int j = 0; j < WORLDSIZE_X; j++)
		{
			if( ( i + j ) % 2 )
				glColor3f( 0, 0, 0 );
			else
				glColor3f( 1, 1, 1 );

			glBegin(GL_QUADS);
			glVertex3f( mWorldPos[j][i].x - mTileHalfSize, mWorldPos[j][i].y, mWorldPos[j][i].z - mTileHalfSize );
			glVertex3f( mWorldPos[j][i].x - mTileHalfSize, mWorldPos[j][i].y, mWorldPos[j][i].z + mTileHalfSize );
			glVertex3f( mWorldPos[j][i].x + mTileHalfSize, mWorldPos[j][i].y, mWorldPos[j][i].z + mTileHalfSize );
			glVertex3f( mWorldPos[j][i].x + mTileHalfSize, mWorldPos[j][i].y, mWorldPos[j][i].z - mTileHalfSize );
			glEnd();
		}
	}
	glPopMatrix();
}

Vector3f World::GetWorldPosition( Vector2i pos )
{
	return mWorldPos[pos.x][pos.y];
}