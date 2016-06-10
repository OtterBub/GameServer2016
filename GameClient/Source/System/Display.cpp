#include "Display.h"
#include "ClientConnect.h"

World Display::mWorld;

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

	// init
	mWorld.Start();

	glutMainLoop();
}

void Display::OpenGLLoop()
{
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
	gluLookAt(0, 100, 500,
		0, 0, 0,
		0, 1, 0);

	glDisable(GL_DEPTH_TEST);
	// UI Draw

	glEnable(GL_DEPTH_TEST);

	char string[300] = { 0, };
	sprintf(string, "( %d, %d )", 100, 100);
	glPushMatrix();
	{
		glColor3f(1, 1, 1);
		glTranslatef(0, 0, 0);
		drawStrokeText(string, 0, 0, 0, 0.08);
	}
	glPopMatrix();

	mWorld.Draw();

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
		//cs_packet_move* movePacket = reinterpret_cast<cs_packet_move*>(CONNECT.GetSendBuffAddr());
		cs_packet_move* movePacket = CONNECT.GetSendBuffAddr<cs_packet_move>();
		movePacket->moveDir = movedir;
		movePacket->header.type = CS_TYPE_MOVE;

		CONNECT.SendPacket(sizeof(movePacket));
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
	using namespace std::chrono;
	static bool init = false;
	static steady_clock::time_point t1 = steady_clock::now();
	static steady_clock::time_point t2 = steady_clock::now();
	static duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	t2 = steady_clock::now();

	if (!init)
	{
		// Initialize
		init = true;
	}
	else
	{
		// Update
		time_span = duration_cast<duration<double>>(t2 - t1);
		double dt = time_span.count();
		mWorld.Update(dt);
	}

	t1 = steady_clock::now();

	glutTimerFunc(100, Timer, 0);
	glutPostRedisplay();
}