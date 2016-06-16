#pragma once
#ifndef __SCENE_MMO_H__
#define __SCENE_MMO_H__

#include "Scene.h"
#include "../ButtonUI.h"

class SceneMMO : public Scene
{
public:
	SceneMMO();
	~SceneMMO();

	void Enter();
	void Exit();
	void Draw();
	void Update(double dt);
	void Reshape(int w, int h);

	void Mouse(int button, int state, int x, int y);
	void MouseMotion(int x, int y);

	/*void KeyBoard( unsigned char key, int x, int y ) {}
	void KeyBoardUp( unsigned char key, int x, int y) {}
	void KeyBoardSpecial( int key, int x, int y ) {}
	void KeyBoardSpecialUp( int key, int x, int y ) {}
	bool Command( std::vector< std::string > commandTokens ) { return false; }*/

private:
	GLdouble wx, wy, wz;
	GLdouble modelView[16];
};

#endif