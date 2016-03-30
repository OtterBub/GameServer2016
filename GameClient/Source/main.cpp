#include "Common.h"
#include "ClientThread.h"
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
Player g_Player[20];
WorldData g_worldData;

int main()
{
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutInitWindowPosition( 100, 100 );
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

	for( int i = 0; i < 20; i++ )
	{
		g_Player[i].Start();
	}

	HANDLE hThread;

	char SERVERIP[20];
	printf( "서버 ip 주소를 입력하시오: " );
	scanf( "%s", SERVERIP );

	HANDLE hLogInComplete;

	hLogInComplete = CreateEvent( NULL, TRUE, FALSE, NULL );
	if( hLogInComplete == NULL )	return -1;


	hThread = CreateThread( NULL, 0, ClientMain, &SERVERIP, 0, NULL );

	CloseHandle( hLogInComplete );

	WaitForSingleObject(hLogInComplete, INFINITE);

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

	for( int i = 0; i < 20; i++ )
	{
		if( g_worldData.playerInfo[i].login )
		{
			g_Player[i].Draw();
		}		
	}

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
	int movedir = 0;
	switch( key )
	{
		case 100: // left
			movedir = MOVE_LEFT;
			break;
		case 101: // up
			movedir = MOVE_UP;
			break;
		case 102: // right
			movedir = MOVE_RIGHT;
			break;
		case 103: // down
			movedir = MOVE_DOWN;
			break;
	}

	if( movedir != 0 )
	{
		// Send Player Key Input to Server
		if( serverSock != NULL )
		send(serverSock, (char*)&movedir, sizeof(movedir), 0);
	}

	//g_Player.SetWorldposition( currentPos );
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

	for( int i = 0; i < 20; i++ )
	{
		if( g_worldData.playerInfo[i].login )
		{
			Vector3f worldPos;
			Vector2i playerPos;
			playerPos.x = g_worldData.playerInfo[i].pos.x;
			playerPos.y = g_worldData.playerInfo[i].pos.y;
			worldPos = g_World.GetWorldPosition( playerPos );
			g_Player[i].SetPosition( worldPos );
		}
	}

	glutTimerFunc( 3, Timer, 0 );
	glutPostRedisplay();
}