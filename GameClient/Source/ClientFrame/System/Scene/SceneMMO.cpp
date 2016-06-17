#include "SceneMMO.h"
#include "../../../System/ClientConnect.h"

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

	memset(mKey, 0, sizeof(mKey));

	cs_packet_login *login = CONNECT.GetSendBuffAddr<cs_packet_login>();

	wchar_t gameid[10];

	std::cout << "GAME ID를 입력하시오: ";
	std::wcin >> gameid;

	login->header.size = sizeof(cs_packet_login);
	login->header.type = CS_LOGIN;
	std::wcscpy(login->nick, gameid);

	CONNECT.SendPacket(sizeof(cs_packet_login));
}
void SceneMMO::Exit()
{
}
void SceneMMO::Draw()
{
	glPushMatrix();
	{
		static int camDist = 20;
		Vector3 myPos = PLAYER(CONNECT.mMyID).GetPosition();
		gluLookAt(myPos.x, 10, myPos.z + camDist, myPos.x, 0, myPos.z, 0, 1, 0);

		glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
		// Draw Ground
		glPushMatrix();
		{
			static int worldSize = 100;
			static Vector4 color = Vector4(0.8, 0.8, 0.8, 0.8);
			static Vector4 reColor = Vector4(1, 1, 1, 1) - color;
			float lGroundSize = 0.5;
			float lGroundHeight = 0;
			for (int y = 0; y < worldSize; ++y)
			{
				for (int x = 0; x < worldSize; ++x)
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

		mTestCube.Draw();
		// Lock Please
		CONNECT.mConnectLock.ReadLock();
		for (auto it = PLAYERMGR.GetList().begin(); it != PLAYERMGR.GetList().end(); ++it)
		{
			PLAYER(it->first).Draw();
		}
		for (auto it = NPCMGR.GetList().begin(); it != NPCMGR.GetList().end(); ++it)
		{
			if( NPC(it->first).mDraw )
				NPC(it->first).Draw();
		}
		
		CONNECT.mConnectLock.ReadUnLock();

		glPushMatrix();
		{
			//glLoadIdentity();
			/*glTranslatef(pickpos.x, pickpos.y, pickpos.z);
			glColor4f( 1, 1, 1, 1 );
			glutSolidCube(10);*/
		}
		glPopMatrix();
	}
	glPopMatrix();
}
void SceneMMO::Update(double dt)
{
	mKeyTime += dt;
	if (mPickCal)
	{
		mTestCube.SetPosition(mPickPos);		

		mPickCal = false;
	}

	//cs_packet_move
	if (mKey['a'] ||
		mKey['d'] ||
		mKey['w'] || 
		mKey['s'] )
	{
		static const float keyDelay = 0.1;
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


	/*while (CONNECT.ThereIsProcessPacket())
	{
		packet_header *header = reinterpret_cast<packet_header*> (&CONNECT.GetPacket());

		switch (header->type)
		{
		case SC_TYPE_MOVE:
		{
			sc_packet_player_pos *posPacket = reinterpret_cast<sc_packet_player_pos*>(header);
			std::string str = "SC_TYPE_MOVE (" +
				std::to_string(posPacket->x) + std::string(", ") +
				std::to_string(posPacket->y) + std::string(")") +
				"[" + std::to_string(posPacket->id) + "]";
			SKCONSOLE << str;

			PLAYER(posPacket->id).SetPosition(Vector3(posPacket->x, 0, posPacket->y));

			break;
		}
		case SC_TYPE_PLAYER_REMOVE:
		{
			sc_packet_player_remove *removePacket = reinterpret_cast<sc_packet_player_remove*>(header);

			std::string str = "SC_TYPE_PLAYER_REMOVE [" + std::to_string(removePacket->id) + "]";
			SKCONSOLE << str;

			PLAYERMGR.DeleteClient(removePacket->id);
			break;
		}
		default:
			std::cout << "UnKnown Packet Type: " << (int)header->type << std::endl;
			break;
		}
	}*/
	
	
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
}
void SceneMMO::KeyBoardUp(unsigned char key, int x, int y)
{
	mKey[key] = false;
}
