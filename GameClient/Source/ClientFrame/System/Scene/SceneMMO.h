#pragma once
#ifndef __SCENE_MMO_H__
#define __SCENE_MMO_H__

#include "Scene.h"
#include "../ButtonUI.h"
#include "../../Object/ObjectOutLineCube.h"
#include "../../Object/ObjectSkier.h"
#include "../MGR/MTObjectMgr.h"

#define PLAYERMGR MTObjectMGR<unsigned int, ObjectSkier>::GetInstance(0)
#define PLAYER(x) MTObjectMGR<unsigned int, ObjectSkier>::GetInstance(0).GetObj(x)

#define NPCMGR MTObjectMGR<unsigned int, ObjectSkier>::GetInstance(1)
#define NPC(x) MTObjectMGR<unsigned int, ObjectSkier>::GetInstance(1).GetObj(x)

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

	void KeyBoard(unsigned char key, int x, int y);
	void KeyBoardUp(unsigned char key, int x, int y);

	bool Command(std::vector< std::string > commandTokens);
	/*
	void KeyBoardSpecial( int key, int x, int y ) {}
	void KeyBoardSpecialUp( int key, int x, int y ) {}
	bool Command( std::vector< std::string > commandTokens ) { return false; }
	*/

	Vector3 PickMouse(int x, int y);

private:
	GLdouble wx, wy, wz;
	GLdouble modelView[16];

	Vector2 mMouseClickPos;
	ObjectOutLineCube mTestCube;

	Vector3 mPickPos;
	bool mMouseLDown;
	bool mPickCal;
	bool mKey[UCHAR_MAX];

	double mKeyTime;
	double mAtkTime;
	bool mKeyDown;
};

#endif