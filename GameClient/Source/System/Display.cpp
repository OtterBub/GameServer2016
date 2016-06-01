#include "Display.h"
#include "../ClientThread.h"

void Display::Initialize()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Study");
	glutDisplayFunc(Draw);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyBoard);
	glutKeyboardUpFunc(KeyBoardUp);
	glutSpecialFunc(SpecialKeyBoard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);

	glutTimerFunc(3, Timer, 0);

	glutMainLoop();
}

GLvoid Display::Draw(GLvoid)
{
	glClearColor(0.1, 0.8, 0.9, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	/*gluLookAt(g_playerCameraPos.x, g_playerCameraPos.y, g_playerCameraPos.z,
		g_playerPos.x, g_playerPos.y, g_playerPos.z,
		0, 1, 0);*/
	gluLookAt(100, 100, 100,
		0, 0, 0,
		0, 1, 0);

	glEnable(GL_DEPTH_TEST);

	// UI Draw

	glDisable(GL_DEPTH_TEST);

	char string[300] = { 0, };
	sprintf(string, "( %d, %d )", 100, 100);
	glPushMatrix();
	{
		glColor3f(1, 1, 1);
		glTranslatef(0, 0, 0);
		drawStrokeText(string, 0, 0, 0, 0.08);
	}
	glPopMatrix();

	glutSwapBuffers();
}
GLvoid Display::Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / (float)h, 1.0, 10000000.0);
	glMatrixMode(GL_MODELVIEW);
}
GLvoid Display::KeyBoard(unsigned char key, int x, int y)
{

}
GLvoid Display::KeyBoardUp(unsigned char key, int x, int y)
{

}
GLvoid Display::SpecialKeyBoard(int key, int x, int y)
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
GLvoid Display::SpecialKeyBoardUp(int key, int x, int y)
{
	printf("%d\n", key);
}
GLvoid Display::Mouse(int button, int state, int x, int y)
{
}
GLvoid Display::MouseMotion(int x, int y)
{
}
GLvoid Display::Timer(int val)
{
	static float deltaTime = 0;
	static float currentTime = clock();

	glutTimerFunc(3, Timer, 0);
	glutPostRedisplay();
}