#ifndef __SCENE_CURVETEST_H__
#define __SCENE_CURVETEST_H__

#include "Scene.h"
#include "../../Object/ObjectOutLineCube.h"
#include "../ButtonUI.h"

class SceneCurveTest : public Scene
{
public:
	SceneCurveTest();
	~SceneCurveTest();

	void Enter();
	void Exit();
	void Draw();
	void Update( double dt );
	void Reshape( int w, int h );

	void KeyBoard( unsigned char key, int x, int y );
	void KeyBoardUp( unsigned char key, int x, int y);
	void Mouse( int button, int state, int x, int y );
	void MouseMotion( int x, int y );
	
	/*
	void KeyBoardSpecial( int key, int x, int y ) {}
	void KeyBoardSpecialUp( int key, int x, int y ) {}
	*/

private:
	int mClientWidth, mClientHeight;
	bool mKey[UCHAR_MAX];
	bool mLeftButtonDown;

	ObjectOutLineCube mResult;
	std::vector< std::shared_ptr< ObjectOutLineCube > > mCubeVec;
	std::vector< Vector3 > mCubePos;
	std::vector< Vector3 > mLineVec;

	ButtonUI mButton;
};

#endif