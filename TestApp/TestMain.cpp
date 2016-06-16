//#include <string>
//#include <thread>
//#include <map>
//#include <stdlib.h>
//#include <mutex>
//#include "../GameServer/Source/System/RWLock.h"
//
//#include "../GameClient/Source/System/ClientConnect.h"
//
//
//
//// Lock Test
//void LockTest()
//{
//	
//}
//
//// CONNECT Test
//void ConnectTest()
//{
//	CONNECT.Initialize();
//
//	CONNECT.Connect( "127.0.0.1" );
//
//	std::thread *readPacketThread = new std::thread{ CONNECT.ReadPacket };
//
//	while (1)
//	{
//		static int move = 0;;
//
//		//cs_packet_move *packet = reinterpret_cast<cs_packet_move*>(CONNECT.GetSendBuffAddr());
//		cs_packet_move *packet = CONNECT.GetSendBuffAddr<cs_packet_move>();
//		
//		packet->header.type = 1;
//		packet->moveDir = ++move;
//
//		CONNECT.SendPacket( sizeof(cs_packet_move) );
//		Sleep(1000);
//	}
//
//	readPacketThread->join();
//	delete readPacketThread;
//}
//
//int main()
//{
//	//ConnectTest();
//	//LockTest;
//
//	return 0;
//}

/*
* Title
glPick by baboneo.cpp
- A Picking Example using OpenGL
* Date
September 16, 2004
* Author
Sekil Park (20042016, skpark@icu.ac.kr), School of Engineering, ICU
http://cgv.icu.ac.kr/
http://blog.naver.com/baboneo

* OS, Compiler, Library, etc
Windows XP, Visual C++ 6.0, OpenGL

* Remarks
-
*/

///////////////////////////////////////////////////////////////////////////////////////////////////

//#include <GL/gl.h>
//#include <GL/glu.h>
#include <glut.h>

#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////

//#pragma comment (linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

///////////////////////////////////////////////////////////////////////////////////////////////////

#define KEY_ESC				27

#define SELECT_BUF_SIZE		512

#define MAX_OBJECT			2
#define TRIANGLE			100
#define QUAD				101

#define SEPARATOR			200

#define MAX_COLOR			9
#define CLR_BLACK			0
#define CLR_WHITE			1
#define CLR_RED				2
#define CLR_GREEN			3
#define CLR_BLUE			4
#define CLR_YELLOW			5
#define CLR_MAGENTA			6
#define CLR_CYAN			7
#define CLR_ORANGE			8

#define WAVE_SPEED			0.0002

///////////////////////////////////////////////////////////////////////////////////////////////////

float g_aColor[MAX_COLOR][3] =
{
	{ 0.0f, 0.0f, 0.0f },	// Black
	{ 1.0f, 1.0f, 1.0f },	// White
	{ 1.0f, 0.0f, 0.0f },	// Red
	{ 0.0f, 1.0f, 0.0f },	// Green
	{ 0.0f, 0.0f, 1.0f },	// Blue
	{ 1.0f, 1.0f, 0.0f },	// Yellow
	{ 1.0f, 0.0f, 1.0f },	// Magenta
	{ 0.0f, 1.0f, 1.0f },	// Cyan
	{ 1.0f, 0.6f, 0.0f }	// Orange
};

short g_nObjectColor[MAX_OBJECT] = { CLR_WHITE, CLR_WHITE };
short g_nSelectedObject = TRIANGLE;

float g_fRatio;

// for Rotation
bool g_bMotion = false;
int g_iMouse[2];
int g_iMousePos[2];

bool g_bRotation = false;
float g_fRotationSpeed = 0.01;

char g_szMsg[100];

float g_fStartX = 0.0, g_fStartY = 0.0, g_fStartZ = 0.0;
float g_fWaveSpeedX = WAVE_SPEED, g_fWaveSpeedY = WAVE_SPEED, g_fWaveSpeedZ = WAVE_SPEED * 5;

///////////////////////////////////////////////////////////////////////////////////////////////////

void InitGL(void);
void Reshape(int w, int h);
void Draw(GLenum eMode);
void DrawTriangle(float fStartX, float fStartY, float fStartZ);
void DrawQuad(float fStartX, float fStartY, float fStartZ);
void DrawColorPicker(GLenum eMode);
void Display(void);
void Idle(void);
void Keyboard(unsigned char ucKey, int x, int y);
void Special(int iValue, int x, int y);
void Mouse(int iButton, int iState, int x, int y);
void PassiveMouse(int x, int y);
void Motion(int x, int y);
void Picking(int x, int y);
void ProcessHits(unsigned int uiHits, unsigned int *pBuffer);
void MakeNoticeLists(void);

///////////////////////////////////////////////////////////////////////////////////////////////////

void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);

	glutCreateWindow("... glPick by baboneo ...");

	InitGL();

	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(PassiveMouse);
	glutMotionFunc(Motion);
	glutIdleFunc(Idle);

	glutMainLoop();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void InitGL(void)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	MakeNoticeLists();
}

void Reshape(int w, int h)
{
	if (!w || !h) return;

	g_fRatio = (float)w / (float)h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-1.15, 1.15, -1.15, 1.15, -5, 5);
	gluPerspective(45.0, g_fRatio, 1.0, 5.0);
	gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Draw(GLenum eMode)
{
	if (eMode == GL_SELECT) glLoadName(TRIANGLE);
	if (TRIANGLE == g_nSelectedObject) DrawTriangle(g_fStartX, g_fStartY, g_fStartZ);
	else DrawTriangle(0, 0, 0);

	if (eMode == GL_SELECT) glLoadName(QUAD);
	if (QUAD == g_nSelectedObject) DrawQuad(g_fStartX, g_fStartY, g_fStartZ);
	else DrawQuad(0, 0, 0);

	DrawColorPicker(eMode);
}

void DrawTriangle(float fStartX, float fStartY, float fStartZ)
{
	glBegin(GL_TRIANGLES);
	glColor3fv(g_aColor[g_nObjectColor[TRIANGLE - TRIANGLE]]);
	glVertex3f(fStartX + -0.3f, fStartY + 0.8f, fStartZ + 0.0f);

	glColor3fv(g_aColor[CLR_WHITE]);
	glVertex3f(fStartX + -0.8f, fStartY + -0.2f, fStartZ + 0.0f);
	glVertex3f(fStartX + 0.2f, fStartY + -0.2f, fStartZ + 0.0f);
	glEnd();

	// Draw Frame
	if (TRIANGLE == g_nSelectedObject)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_TRIANGLES);
		glColor3fv(g_aColor[CLR_ORANGE]);
		glVertex3f(fStartX + -0.3f, fStartY + 0.85f, fStartZ + 0.0f);

		glColor3fv(g_aColor[CLR_WHITE]);
		glVertex3f(fStartX + -0.85f, fStartY + -0.23f, fStartZ + 0.0f);
		glVertex3f(fStartX + 0.25f, fStartY + -0.23f, fStartZ + 0.0f);
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void DrawQuad(float fStartX, float fStartY, float fStartZ)
{
	glBegin(GL_QUADS);
	glColor3fv(g_aColor[g_nObjectColor[QUAD - TRIANGLE]]);
	glVertex3f(fStartX + 0.8f, fStartY + 0.8f, fStartZ + -1.0f);
	glVertex3f(fStartX + 0.3f, fStartY + 0.8f, fStartZ + -1.0f);

	glColor3fv(g_aColor[CLR_WHITE]);
	glVertex3f(fStartX + 0.3f, fStartY + -0.2f, fStartZ + 0.0f);
	glVertex3f(fStartX + 0.8f, fStartY + -0.2f, fStartZ + 0.0f);
	glEnd();

	// Draw Frame
	if (QUAD == g_nSelectedObject)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_QUADS);
		glColor3fv(g_aColor[CLR_ORANGE]);
		glVertex3f(fStartX + 0.83f, fStartY + 0.83f, fStartZ + -1.0f);
		glVertex3f(fStartX + 0.27f, fStartY + 0.83f, fStartZ + -1.0f);

		glColor3fv(g_aColor[CLR_WHITE]);
		glVertex3f(fStartX + 0.27f, fStartY + -0.23f, fStartZ + 0.0f);
		glVertex3f(fStartX + 0.83f, fStartY + -0.23f, fStartZ + 0.0f);
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void DrawColorPicker(GLenum eMode)
{
	// Separator
	if (eMode == GL_SELECT) glLoadName(SEPARATOR);

	glLineWidth(5);
	glColor3fv(g_aColor[CLR_WHITE]);
	glBegin(GL_LINES);
	glVertex3f(-1.1f, -0.45f, 0.0f);
	glVertex3f(1.1f, -0.45f, 0.0f);
	glEnd();

	// Color Picker
	glLineWidth(1);

	// Red
	if (eMode == GL_SELECT) glLoadName(CLR_RED);
	glColor3fv(g_aColor[CLR_RED]);
	glBegin(GL_QUADS);
	glVertex3f(-0.65f, -0.65f, 0.0f);
	glVertex3f(-0.85f, -0.65f, 0.0f);
	glVertex3f(-0.85f, -0.85f, 0.0f);
	glVertex3f(-0.65f, -0.85f, 0.0f);
	glEnd();

	// Green
	if (eMode == GL_SELECT) glLoadName(CLR_GREEN);
	glColor3fv(g_aColor[CLR_GREEN]);
	glBegin(GL_QUADS);
	glVertex3f(-0.35f, -0.65f, 0.0f);
	glVertex3f(-0.55f, -0.65f, 0.0f);
	glVertex3f(-0.55f, -0.85f, 0.0f);
	glVertex3f(-0.35f, -0.85f, 0.0f);
	glEnd();

	// Blue
	if (eMode == GL_SELECT) glLoadName(CLR_BLUE);
	glColor3fv(g_aColor[CLR_BLUE]);
	glBegin(GL_QUADS);
	glVertex3f(-0.05f, -0.65f, 0.0f);
	glVertex3f(-0.25f, -0.65f, 0.0f);
	glVertex3f(-0.25f, -0.85f, 0.0f);
	glVertex3f(-0.05f, -0.85f, 0.0f);
	glEnd();

	// Yellow
	if (eMode == GL_SELECT) glLoadName(CLR_YELLOW);
	glColor3fv(g_aColor[CLR_YELLOW]);
	glBegin(GL_QUADS);
	glVertex3f(0.25f, -0.65f, 0.0f);
	glVertex3f(0.05f, -0.65f, 0.0f);
	glVertex3f(0.05f, -0.85f, 0.0f);
	glVertex3f(0.25f, -0.85f, 0.0f);
	glEnd();

	// Magenta
	if (eMode == GL_SELECT) glLoadName(CLR_MAGENTA);
	glColor3fv(g_aColor[CLR_MAGENTA]);
	glBegin(GL_QUADS);
	glVertex3f(0.55f, -0.65f, 0.0f);
	glVertex3f(0.35f, -0.65f, 0.0f);
	glVertex3f(0.35f, -0.85f, 0.0f);
	glVertex3f(0.55f, -0.85f, 0.0f);
	glEnd();

	// Cyan
	if (eMode == GL_SELECT) glLoadName(CLR_CYAN);
	glColor3fv(g_aColor[CLR_CYAN]);
	glBegin(GL_QUADS);
	glVertex3f(0.85f, -0.65f, 0.0f);
	glVertex3f(0.65f, -0.65f, 0.0f);
	glVertex3f(0.65f, -0.85f, 0.0f);
	glVertex3f(0.85f, -0.85f, 0.0f);
	glEnd();
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (g_bRotation) glRotated(g_fRotationSpeed, 0.0, 1.0, 0.0);

	Draw(GL_RENDER);

	// Text
	glPushMatrix();
	glLoadIdentity();

	glColor3fv(g_aColor[CLR_YELLOW]);
	glRasterPos2f(-1.15, 1.15);
	sprintf(g_szMsg, "* Mouse Position : %03d, %03d", g_iMousePos[0], g_iMousePos[1]);
	glCallLists((GLint)strlen(g_szMsg), GL_BYTE, g_szMsg);

	glColor3fv(g_aColor[CLR_WHITE]);
	glRasterPos2f(-1.15, 1.05);
	sprintf(g_szMsg, "* Control : 3 Mouse Buttons, R and Up/Down Arrow Keys!");
	glCallLists((GLint)strlen(g_szMsg), GL_BYTE, g_szMsg);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void Idle(void)
{
	g_fStartX += g_fWaveSpeedX;
	g_fStartY += g_fWaveSpeedY;
	g_fStartZ += g_fWaveSpeedZ;

	if (g_fStartX >= 0.1) g_fWaveSpeedX = -WAVE_SPEED;
	if (g_fStartX <= -0.1) g_fWaveSpeedX = WAVE_SPEED;

	if (g_fStartY >= 0.1) g_fWaveSpeedY = -WAVE_SPEED;
	if (g_fStartY <= 0.0) g_fWaveSpeedY = WAVE_SPEED;

	if (g_fStartZ >= 1.0) g_fWaveSpeedZ = -WAVE_SPEED * 5;
	if (g_fStartZ <= -1.0) g_fWaveSpeedZ = WAVE_SPEED * 5;

	glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Keyboard(unsigned char ucKey, int x, int y)
{
	switch (ucKey)
	{
	case KEY_ESC:
		exit(0);
		break;

	case 'r':
	case 'R':
		g_bRotation = !g_bRotation;
		break;
	}
}

void Special(int iValue, int x, int y)
{
	switch (iValue)
	{
	case GLUT_KEY_UP:
		g_fRotationSpeed += 0.01;
		break;

	case GLUT_KEY_DOWN:
		g_fRotationSpeed -= 0.01;
		break;

		//default :
		//	break;
	}
}

void Mouse(int iButton, int iState, int x, int y)
{
	if (iState == GLUT_DOWN)
	{
		switch (iButton)
		{
		case GLUT_LEFT_BUTTON:
			Picking(x, y);
			break;

		case GLUT_RIGHT_BUTTON:
			//if(glutGetModifiers() == GLUT_ACTIVE_CTRL)
			g_bMotion = true;
			g_iMouse[0] = x;
			g_iMouse[1] = y;
			break;
		}
	}
	else if (iState == GLUT_UP)
	{
		switch (iButton)
		{
		case GLUT_RIGHT_BUTTON:
			g_bMotion = false;
			break;

		case GLUT_MIDDLE_BUTTON:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			//glOrtho(-1.15, 1.15, -1.15, 1.15, -5, 5);
			gluPerspective(45.0, g_fRatio, 1.0, 5.0);
			gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			break;
		}
	}
}

void PassiveMouse(int x, int y)
{
	g_iMousePos[0] = x;
	g_iMousePos[1] = y;
}

void Motion(int x, int y)
{
	int dx, dy;

	if (g_bMotion)
	{
		dx = g_iMouse[0] - x;
		dy = g_iMouse[1] - y;

		glRotated(dy, 1.0, 0.0, 0.0);
		glRotated(dx, 0.0, 1.0, 0.0);

		g_iMouse[0] = x;
		g_iMouse[1] = y;
	}

	g_iMousePos[0] = x;
	g_iMousePos[1] = y;

	glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Picking(int x, int y)
{
	static unsigned int aSelectBuffer[SELECT_BUF_SIZE];
	static unsigned int uiHits;
	static int aViewport[4];

	glGetIntegerv(GL_VIEWPORT, aViewport);

	glSelectBuffer(SELECT_BUF_SIZE, aSelectBuffer);
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// 5x5 Region
	gluPickMatrix((double)x, (double)(aViewport[3] - y), 5.0, 5.0, aViewport);

	// Same Clipping Window as in Reshape()
	//glOrtho(-1.15, 1.15, -1.15, 1.15, -5, 5);
	gluPerspective(45.0, g_fRatio, 1.0, 5.0);
	gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glTranslated(0.5, 0, 0);
	// Draw!
	Draw(GL_SELECT);
	//glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	uiHits = glRenderMode(GL_RENDER);
	ProcessHits(uiHits, aSelectBuffer);

	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void ProcessHits(unsigned int uiHits, unsigned int *pBuffer)
{
	static unsigned int i, j;
	static unsigned int uiName, *ptr;

	ptr = pBuffer;
	for (i = 0; i < uiHits; i++)			// for each hit
	{
		uiName = *ptr;
		ptr += 3;

		for (j = 0; j < uiName; j++)		// for each name
		{
			switch (*ptr)
			{
			case TRIANGLE:
				g_nSelectedObject = TRIANGLE;

				printf("[ Triangle ]\n");
				break;

			case QUAD:
				g_nSelectedObject = QUAD;

				printf("[ Quad ]\n");
				break;

			case SEPARATOR:
				printf("--- Separator ---\n");
				break;

			case CLR_RED:
				g_nObjectColor[g_nSelectedObject - TRIANGLE] = CLR_RED;

				printf("< Red >\n");
				break;

			case CLR_GREEN:
				g_nObjectColor[g_nSelectedObject - TRIANGLE] = CLR_GREEN;

				printf("< Green >\n");
				break;

			case CLR_BLUE:
				g_nObjectColor[g_nSelectedObject - TRIANGLE] = CLR_BLUE;

				printf("< Blue >\n");
				break;

			case CLR_YELLOW:
				g_nObjectColor[g_nSelectedObject - TRIANGLE] = CLR_YELLOW;

				printf("< Yellow >\n");
				break;

			case CLR_MAGENTA:
				g_nObjectColor[g_nSelectedObject - TRIANGLE] = CLR_MAGENTA;

				printf("< Magenta >\n");
				break;

			case CLR_CYAN:
				g_nObjectColor[g_nSelectedObject - TRIANGLE] = CLR_CYAN;

				printf("< Cyan >\n");
				break;
			}

			ptr++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void MakeNoticeLists(void)
{
	GLuint base = glGenLists(256);

	for (int i = 0; i < 256; i++)
	{
		glNewList(base + i, GL_COMPILE);
		//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, i);
		//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, i);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, i);
		glEndList();
	}

	glListBase(base);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
