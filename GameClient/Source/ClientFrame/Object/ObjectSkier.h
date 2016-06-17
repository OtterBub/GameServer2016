#ifndef __OBJECT_SKIER_H__
#define __OBJECT_SKIER_H__

#include "../../Common.h"
#include "ObjectOutLineCube.h"

#include <list>

class ObjectSkier : public Object
{
public:
	ObjectSkier();
	~ObjectSkier();

	void Draw();
	void Update( double dt );
	void Reshape( int w, int h );

	void SetPosition( Vector3 pos );
	void SetDest( std::list< Vector3 > listDest );
	void AddDest( std::list< Vector3 > listDest );
	void AddDest( Vector3 listDest );
	void SetColor(Vector4 color);

	void SetCollisionTree( bool set );
	void SetAutoRotate( bool set );
	void SetSmooth( bool set );
	void SetVelocityAndIncrement( float initVel, float inc );

	Vector3 GetPosition();
	Vector3 GetMidPosition();
	float GetSize();
	bool GetCollisionTree();

public:
	WORD id;
	WORD x_pos;
	WORD y_pos;
	WORD HP;
	WORD LEVEL;
	WORD EXP;

protected:
	Vector3 GetNextDest();
	void Move( double dt );
	void Rotate();

protected:
	Vector3 mPosition;
	Vector3 mDest;
	Vector3 mDir;
	float mInitVel;
	float mVelocity;
	float mVelIncrement;
	float mDestDistance;
	float mLeftDistance;
	float mAngleY;
	float mAngleZ;
	bool mSetDest;
	bool mAutoRotate;
	bool mSetCollisionTree;
	bool mSmoothFinish;

	double mAttackTime;
	bool mAttack;

	std::list< Vector3 > mListDest;

	ObjectOutLineCube mMainBody;
};

#endif