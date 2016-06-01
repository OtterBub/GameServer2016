#include "Common.h"
#include "ClientThread.h"
#include "Object\World.h"
#include "Object\Player.h"



void drawStrokeText(char *string, float x, float y, float z, float scale) {
	char *c;
	glPushMatrix();
	{
		glTranslatef(x, y, z);
		//glColor3f(1.0, 1.0, 1.0);
		glScalef(scale, scale, scale);
		for (c = string; *c != '\0'; c++)
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *c);
	}
	glPopMatrix();
}

GLvoid Display(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid KeyBoardUp(unsigned char key, int x, int y);
GLvoid SpecialKeyBoard(int key, int x, int y);
GLvoid SpecialKeyBoardUp(int key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid MouseMotion(int x, int y);
GLvoid Timer(int val);

static World g_World;
Player g_Player[20];
WorldData g_worldData;

Vector3f g_playerCameraPos;
Vector3f g_playerPos;

float distCameraZ = 130;
float distCameraY = 100;

int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Study");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyBoard);
	glutKeyboardUpFunc(KeyBoardUp);
	glutSpecialFunc(SpecialKeyBoard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);

	glutTimerFunc(3, Timer, 0);
	srand((unsigned int)time(NULL));

	g_World.Start();
	for (int i = 0; i < 20; i++)
	{
		g_Player[i].Start();
	}

	g_playerCameraPos.x = 70;
	g_playerCameraPos.y = 100;
	g_playerCameraPos.z = 200;

	g_playerPos.x = 70;
	g_playerPos.y = 0;
	g_playerPos.z = 70;

	HANDLE hThread;

	char SERVERIP[20];
	char gameid[255];
	char gamepass[255];
	printf("서버 ip 주소를 입력하시오: ");
	scanf("%s", SERVERIP);

	printf("GAME ID를 입력하시오: ");
	scanf("%s", gameid);

	printf("GAME PASSWORD를 입력하시오: ");
	scanf("%s", gamepass);

	HANDLE hLogInComplete;

	hLogInComplete = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hLogInComplete == NULL)	return -1;


	hThread = CreateThread(NULL, 0, ClientMain, &SERVERIP, 0, NULL);

	CloseHandle(hLogInComplete);

	WaitForSingleObject(hLogInComplete, INFINITE);

	glutMainLoop();

	return 0;
}

GLvoid Display(GLvoid)
{
	glClearColor(0.1, 0.8, 0.9, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(g_playerCameraPos.x, g_playerCameraPos.y, g_playerCameraPos.z,
		g_playerPos.x, g_playerPos.y, g_playerPos.z,
		0, 1, 0);

	glEnable(GL_DEPTH_TEST);

	
	Vector2i playerPos = { 0, };
	if (playerID != -1)
	{
		
		playerPos.x = g_worldData.playerInfo[playerID].pos.x;
		playerPos.y = g_worldData.playerInfo[playerID].pos.y;
		g_World.Draw(playerPos, 5);
	}
	else
		g_World.Draw();

	for (int i = 0; i < 20; i++)
	{
		if (g_worldData.playerInfo[i].view && g_worldData.playerInfo[i].login)
		{
			g_Player[i].Draw(  );
		}
	}

	// UI Draw

	glDisable(GL_DEPTH_TEST);

	char string[300] = { 0, };
	sprintf(string, "( %d, %d )", playerPos.x, playerPos.y );
	glPushMatrix();
	{
		glColor3f( 1,1,1 );
		glTranslatef(g_playerPos.x, g_playerPos.y, g_playerPos.z);
		drawStrokeText(string, 0, 0, 0, 0.08);
	}
	glPopMatrix();

	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / (float)h, 1.0, 10000000.0);
	glMatrixMode(GL_MODELVIEW);
}
GLvoid KeyBoard(unsigned char key, int x, int y)
{

}
GLvoid KeyBoardUp(unsigned char key, int x, int y)
{

}
GLvoid SpecialKeyBoard(int key, int x, int y)
{
	unsigned int movedir = 0;
	switch (key)
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

	if (movedir != 0)
	{
		// Send Player Key Input to Server
		if (serverSock != NULL) {
			WSABUF send_wsabuf;
			DWORD iobyte;
			char sendbuffer[255];

			cs_packet_move *movePacket = reinterpret_cast<cs_packet_move*>(sendbuffer);
			movePacket->header.size = sizeof(movePacket);
			movePacket->header.type = CS_TYPE_MOVE;
			movePacket->moveDir = movedir;

			send_wsabuf.buf = sendbuffer;
			send_wsabuf.len = sizeof(movePacket);

			WSASend(serverSock, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);

			send_wsabuf.buf = 0;
			send_wsabuf.len = 0;
		}
	}

	//g_Player.SetWorldposition( currentPos );
}
GLvoid SpecialKeyBoardUp(int key, int x, int y)
{
	printf("%d\n", key);
}
GLvoid Mouse(int button, int state, int x, int y)
{
}
GLvoid MouseMotion(int x, int y)
{
}
GLvoid Timer(int val)
{
	static float deltaTime = 0;
	static float currentTime = clock();

	for (int i = 0; i < 20; i++)
	{
		if (g_worldData.playerInfo[i].login)
		{
			Vector3f worldPos;
			Vector2i playerPos;
			playerPos.x = g_worldData.playerInfo[i].pos.x;
			playerPos.y = g_worldData.playerInfo[i].pos.y;
			worldPos = g_World.GetWorldPosition(playerPos);
			g_Player[i].SetPosition(worldPos);

			if (i == playerID)
			{
				g_playerPos = g_Player[playerID].GetPlayerPos();
				g_playerCameraPos = g_playerPos;
				g_playerCameraPos.y = distCameraY;
				g_playerCameraPos.z += distCameraZ;
			}
		}
	}

	glutTimerFunc(3, Timer, 0);
	glutPostRedisplay();
}