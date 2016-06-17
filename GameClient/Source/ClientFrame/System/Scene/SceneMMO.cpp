#include "SceneMMO.h"
#include "../../../System/ClientConnect.h"
#include "SceneMainMenu.h"

SceneMMO::SceneMMO()
{
	mSceneName = "MMO";
}
SceneMMO::~SceneMMO()
{
}

void SceneMMO::Enter()
{
	SCENEMGR_INST->SetPerspectiveMode(true);

	mTestCube.SetColor(Vector4(0.5, 0.5, 1, 1));
	mMouseLDown = false;
	mPickCal = false;
	mKeyTime = 0.0;
	mAtkTime = 0.0;

	memset(mKey, 0, sizeof(mKey));

	SKCONSOLE << "------- Input ID Help -------";
	SKCONSOLE << "/login Your_id";
}
void SceneMMO::Exit()
{
}
void SceneMMO::Draw()
{
	glPushMatrix();
	{
		Vector3 myPos;
		static int camDist = 15;
		static int camHeight = 10;
		if (CONNECT.mLogin)
		{
			myPos = PLAYER(CONNECT.mMyID).GetPosition();
			gluLookAt(myPos.x, camHeight, myPos.z + camDist, myPos.x, 0, myPos.z, 0, 1, 0);
		}
		else
		{
			myPos = Vector3(50, 0, 50);
			gluLookAt(myPos.x, camHeight, myPos.z + camDist, myPos.x, 0, myPos.z, 0, 1, 0);
		}

		glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
		// Draw Ground
		glPushMatrix();
		{
			static int worldSize = 300;
			static Vector4 color = Vector4(0.8, 0.8, 0.8, 0.8);
			static Vector4 reColor = Vector4(1, 1, 1, 1) - color;
			float lGroundSize = 0.5f;
			float lGroundHeight = 0;
			for (int y = myPos.y - 50; y < myPos.y + 50; ++y)
			{
				for (int x = myPos.x - 50; x < myPos.x + 50; ++x)
				{
					
					glPushMatrix();
					glTranslatef( x, 0, y );
					if ((x + y) % 2)
					{
						glBegin(GL_QUADS);
						glColor4fv(reinterpret_cast<GLfloat*>(&color));
						glVertex3fv(reinterpret_cast<GLfloat*>(&Vector3(lGroundSize, lGroundHeight, lGroundSize)));
						glColor4fv(reinterpret_cast<GLfloat*>(&color));
						glVertex3fv(reinterpret_cast<GLfloat*>(&Vector3(-lGroundSize, lGroundHeight, lGroundSize)));
						glColor4fv(reinterpret_cast<GLfloat*>(&color));
						glVertex3fv(reinterpret_cast<GLfloat*>(&Vector3(-lGroundSize, lGroundHeight, -lGroundSize)));
						glColor4fv(reinterpret_cast<GLfloat*>(&color));
						glVertex3fv(reinterpret_cast<GLfloat*>(&Vector3(lGroundSize, lGroundHeight, -lGroundSize)));
						glEnd();
					}
					else
					{
						glBegin(GL_QUADS);
						glColor4fv(reinterpret_cast<GLfloat*>(&reColor));
						glVertex3fv(reinterpret_cast<GLfloat*>(&Vector3(lGroundSize, lGroundHeight, lGroundSize)));
						glColor4fv(reinterpret_cast<GLfloat*>(&reColor));
						glVertex3fv(reinterpret_cast<GLfloat*>(&Vector3(-lGroundSize, lGroundHeight, lGroundSize)));
						glColor4fv(reinterpret_cast<GLfloat*>(&reColor));
						glVertex3fv(reinterpret_cast<GLfloat*>(&Vector3(-lGroundSize, lGroundHeight, -lGroundSize)));
						glColor4fv(reinterpret_cast<GLfloat*>(&reColor));
						glVertex3fv(reinterpret_cast<GLfloat*>(&Vector3(lGroundSize, lGroundHeight, -lGroundSize)));
						glEnd();
					}
					glPopMatrix();
				}
			}
		}
		glPopMatrix();

		if(mMouseLDown)
			mPickPos = PickMouse( mMouseClickPos.x, mMouseClickPos.y );

		//mTestCube.Draw();
		// Lock Please
		CONNECT.mConnectLock.ReadLock();
		for (auto it = PLAYERMGR.GetList().begin(); it != PLAYERMGR.GetList().end(); ++it)
		{
			PLAYER(it->first).Draw();
		}
		for (auto it = NPCMGR.GetList().begin(); it != NPCMGR.GetList().end(); ++it)
		{
			NPC(it->first).Draw();
		}
		
		CONNECT.mConnectLock.ReadUnLock();
	}
	glPopMatrix();

	glPushMatrix();
	{
		// UI Coordinate
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, SCENEMGR_INST->GetClientSize().x, -SCENEMGR_INST->GetClientSize().y, 0, -1, 1);

		// Depth Test Off
		glDisable(GL_DEPTH_TEST);

		// Light Off
		glDisable(GL_LIGHTING);

		// Console BackGround Draw
		// Blend On
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glColor4f(0.7, 0.7, 0.7, 0.8);
		glRectf(0, -SCENEMGR_INST->GetClientSize().y + 10, SCENEMGR_INST->GetClientSize().x, -SCENEMGR_INST->GetClientSize().y + 50);

		// Blend Off
		glDisable(GL_BLEND);

		// Draw Strings
		if (CONNECT.mLogin)
		{
			int lStrNum = 1;
			std::list<std::string> stringList;
			std::string str = "LEVEL: " + std::to_string(PLAYER(CONNECT.mMyID).LEVEL);
			stringList.push_back(str);
			str = "EXP: " + std::to_string(PLAYER(CONNECT.mMyID).EXP) + "/ " + std::to_string( 100 * (int)pow(2, PLAYER(CONNECT.mMyID).LEVEL));
			
			stringList.push_back(str);
			str = "HP: " + std::to_string(PLAYER(CONNECT.mMyID).HP);
			stringList.push_back(str);
			str = "POS: (" + std::to_string((int)PLAYER(CONNECT.mMyID).GetPosition().x);
			str += ", " + std::to_string((int)PLAYER(CONNECT.mMyID).GetPosition().y);
			str += ")";
			stringList.push_back(str);

			for (std::string it : stringList)
			{
				glLineWidth(2);
				glColor4f(1, 0.8, 0.8, 1);
				DrawStrokeText(it.c_str(), 1, -(25 * lStrNum), 0, 0.150);
				lStrNum++;
			}
		}

		// Restore LineWidth 
		glLineWidth(1);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		// Restore Depth Test
		glEnable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}
void SceneMMO::Update(double dt)
{
	mAtkTime += dt;
	mKeyTime += dt;
	if (mPickCal)
	{
		mTestCube.SetPosition(mPickPos);		

		mPickCal = false;
	}

	CONNECT.mConnectLock.ReadLock();
	for (auto it = PLAYERMGR.GetList().begin(); it != PLAYERMGR.GetList().end(); ++it)
	{
		PLAYER(it->first).Update(dt);
	}
	for (auto it = NPCMGR.GetList().begin(); it != NPCMGR.GetList().end(); ++it)
	{
		NPC(it->first).Update(dt);
	}

	CONNECT.mConnectLock.ReadUnLock();
	

	if (CONNECT.mLogin)
	{
		//cs_packet_move
		if (mKey['a'] ||
			mKey['d'] ||
			mKey['w'] ||
			mKey['s'])
		{
			static const float keyDelay = 0.8;
			if (mKeyTime >= keyDelay)
			{
				mKeyTime = 0;
				BYTE lDir = 0;

				if (mKey['a'])
				{
					lDir |= moveDir::MOVE_LEFT;
				}
				if (mKey['d'])
				{
					lDir |= moveDir::MOVE_RIGHT;
				}
				if (mKey['w'])
				{
					lDir |= moveDir::MOVE_UP;
				}
				if (mKey['s'])
				{
					lDir |= moveDir::MOVE_DOWN;
				}

				if (lDir)
				{
					cs_packet_move *movePacket = CONNECT.GetSendBuffAddr<cs_packet_move>();
					movePacket->moveDir = lDir;
					movePacket->header.type = CS_MOVE;

					CONNECT.SendPacket(sizeof(cs_packet_move));
				}
			}
		}
	}	
}
void SceneMMO::Reshape(int w, int h)
{
}

void SceneMMO::Mouse(int button, int state, int x, int y)
{
	/*
	Mouse buttons.
	#define GLUT_LEFT_BUTTON		0
	#define GLUT_MIDDLE_BUTTON		1
	#define GLUT_RIGHT_BUTTON		2

	Mouse button  state.
	#define GLUT_DOWN			0
	#define GLUT_UP				1
	*/

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mMouseLDown = true;
			mMouseClickPos.x = x;
			mMouseClickPos.y = y;
		}
	}
}
void SceneMMO::MouseMotion(int x, int y)
{
}

Vector3 SceneMMO::PickMouse(int x, int y)
{
	GLdouble projection[16];
	GLint viewPort[4];

	GLfloat z_cursor;
	GLfloat winX, winY;

	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	//glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetIntegerv(GL_VIEWPORT, viewPort);

	winX = (float)x;
	winY = (float)viewPort[3] - (float)y /*- 1*/;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_cursor);

	gluUnProject(winX, winY, z_cursor,
		modelView, projection, viewPort, &wx, &wy, &wz);

	mPickCal = true;
	mMouseLDown = false;

	return Vector3(wx, wy, wz);
}

void SceneMMO::KeyBoard(unsigned char key, int x, int y)
{
	mKey[key] = true;
	static const float atkDelay = 1;
	if (mKey[' '] && CONNECT.mLogin )
	{
		if (mAtkTime >= atkDelay)
		{
			mAtkTime = 0;
			cs_packet_attack *attack = CONNECT.GetSendBuffAddr<cs_packet_attack>();
			attack->header.size = sizeof(cs_packet_attack);
			attack->header.type = CS_ATTACK;
			CONNECT.SendPacket(sizeof(cs_packet_attack));
			SKCONSOLE << "Attack!";
		}
	}
}
void SceneMMO::KeyBoardUp(unsigned char key, int x, int y)
{
	mKey[key] = false;
}

bool SceneMMO::Command(std::vector< std::string > commandTokens)
{
	if (commandTokens.size() >= 2)
	{
		if (commandTokens[0] == "login")
		{
			cs_packet_login *login = CONNECT.GetSendBuffAddr<cs_packet_login>();

			login->header.size = sizeof(cs_packet_login);
			login->header.type = CS_LOGIN;
			size_t i;
			mbstowcs_s(&i, login->nick, 10, commandTokens[1].c_str(), 10);

			CONNECT.SendPacket(sizeof(cs_packet_login));
		}
	}
	else if (commandTokens.size() >= 1)
	{
		if (commandTokens[0] == "logout")
		{
			packet_header *logout = CONNECT.GetSendBuffAddr<packet_header>();

			logout->size = sizeof(cs_packet_login);
			logout->type = CS_LOGOUT;

			CONNECT.mLogin = false;
			CONNECT.SendPacket(sizeof(packet_header));

			SCENEMGR_INST->SceneChange(std::shared_ptr< SceneMainMenu >(new SceneMainMenu()));
		}
		
	}
	return true;
}