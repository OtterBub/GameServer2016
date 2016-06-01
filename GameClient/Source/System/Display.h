#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include "../Common.h"
#include <glut.h>

class Display
{
public:
	static void Initialize();

	static GLvoid Draw(GLvoid);
	static GLvoid Reshape(int w, int h);
	static GLvoid KeyBoard(unsigned char key, int x, int y);
	static GLvoid KeyBoardUp(unsigned char key, int x, int y);
	static GLvoid SpecialKeyBoard(int key, int x, int y);
	static GLvoid SpecialKeyBoardUp(int key, int x, int y);
	static GLvoid Mouse(int button, int state, int x, int y);
	static GLvoid MouseMotion(int x, int y);
	static GLvoid Timer(int val);

private:
	/*static World g_World;
	static Player g_Player[20];
	static WorldData g_worldData;

	static Vector3f g_playerCameraPos;
	static Vector3f g_playerPos;

	static float distCameraZ;
	static float distCameraY;*/
};

#endif