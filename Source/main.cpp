#include "Common.h"
#include "Object\World.h"
#include "Object\Player.h"

void drawStrokeText( char *string, float x, float y, float z, float scale ){
	char *c;
	glPushMatrix();
	{
		glTranslatef( x, y, z );
		glColor3f( 1.0, 1.0, 1.0 );
		glScalef( scale, scale, scale );
		for( c = string; *c != '\0'; c++ )
			glutStrokeCharacter( GLUT_STROKE_ROMAN, *c );
	}
	glPopMatrix();
}

GLvoid Display( GLvoid );
GLvoid Reshape( int w, int h );
GLvoid KeyBoard( unsigned char key, int x, int y );
GLvoid KeyBoardUp( unsigned char key, int x, int y );
GLvoid SpecialKeyBoard( int key, int x, int y );
GLvoid SpecialKeyBoardUp( int key, int x, int y );
GLvoid Mouse( int button, int state, int x, int y );
GLvoid MouseMotion( int x, int y );
GLvoid Timer( int val );

static World g_World;
static Player g_Player;

int main()
{
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "OpenGL Study" );
	glutDisplayFunc( Display );
	glutReshapeFunc( Reshape );
	glutKeyboardFunc( KeyBoard );
	glutKeyboardUpFunc( KeyBoardUp );
	glutSpecialFunc( SpecialKeyBoard );
	glutMouseFunc( Mouse );
	glutMotionFunc( MouseMotion );

	glutTimerFunc( 3, Timer, 0 );
	srand( (unsigned int)time( NULL ) );

	g_World.Start();
	g_Player.Start();

	glutMainLoop();

	return 0;
}

GLvoid Display( GLvoid )
{
	glClearColor( 0.1, 0.8, 0.9, 1 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLoadIdentity();
	gluLookAt( 70, 100, 200, 70, 0, 70, 0, 1, 0 );

	glEnable( GL_DEPTH_TEST );

	g_World.Draw();
	g_Player.Draw();

	glutSwapBuffers();
}
GLvoid Reshape( int w, int h )
{
	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60, (float)w / (float)h, 1.0, 10000000.0 );
	glMatrixMode( GL_MODELVIEW );
}
GLvoid KeyBoard( unsigned char key, int x, int y )
{
	
}
GLvoid KeyBoardUp( unsigned char key, int x, int y )
{
	
}
GLvoid SpecialKeyBoard( int key, int x, int y )
{
	Vector2i currentPos = g_Player.GetWorldPos();
	switch( key )
	{
		case 100: // left
			currentPos.x --;
			if( currentPos.x < 0 )
				currentPos.x = 0;
			break;
		case 101: // up
			currentPos.y --;
			if( currentPos.y < 0 )
				currentPos.y = 0;
			break;
		case 102: // right
			currentPos.x ++;
			if( currentPos.x > 7 )
				currentPos.x = 7;
			break;
		case 103: // down
			currentPos.y ++;
			if( currentPos.y > 7 )
				currentPos.y = 7;
			break;
	}

	g_Player.SetWorldposition( currentPos );
}
GLvoid SpecialKeyBoardUp( int key, int x, int y )
{
	printf( "%d\n", key );
}
GLvoid Mouse( int button, int state, int x, int y )
{
}
GLvoid MouseMotion( int x, int y )
{
}
GLvoid Timer( int val )
{
	static float deltaTime = 0;
	static float currentTime = clock();

	Vector3f currentWorldPos = g_World.GetWorldPosition( g_Player.GetWorldPos() );
	g_Player.SetPosition( currentWorldPos );

	glutTimerFunc( 3, Timer, 0 );
	glutPostRedisplay();
}