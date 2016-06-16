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

	memset(mKey, 0, sizeof(mKey));
}
void SceneMMO::Exit()
{
}
void SceneMMO::Draw()
{
	glPushMatrix();
	{
		gluLookAt(0, 100, 300, 0, 0, 0, 0, 1, 0);

		glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
		// Draw Ground
		glPushMatrix();
		{
			float lGroundSize = 150;
			float lGroundHeight = -80;
			glBegin(GL_QUADS);
			glColor4f(0.5, 0, 0.5, 1);
			glVertex3fv(reinterpret_cast< GLfloat* >(&Vector3(lGroundSize, lGroundHeight, lGroundSize)));
			glColor4f(0.5, 0, 1, 1);
			glVertex3fv(reinterpret_cast< GLfloat* >(&Vector3(-lGroundSize, lGroundHeight, lGroundSize)));
			glColor4f(1, 0, 0.5, 1);
			glVertex3fv(reinterpret_cast< GLfloat* >(&Vector3(-lGroundSize, lGroundHeight, -lGroundSize)));
			glColor4f(0.5, 1, 0.5, 1);
			glVertex3fv(reinterpret_cast< GLfloat* >(&Vector3(lGroundSize, lGroundHeight, -lGroundSize)));
			glEnd();
		}
		glPopMatrix();

		if(mMouseLDown)
			mPickPos = PickMouse( mMouseClickPos.x, mMouseClickPos.y );

		//mTestCube.Draw();
		for (auto it = PLAYERMGR.GetList().begin(); it != PLAYERMGR.GetList().end(); ++it)
		{
			PLAYER(it->first).Draw();
		}

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
	if (mPickCal)
	{
		mTestCube.SetPosition(mPickPos);		
		cs_packet_player_pos *packet = CONNECT.GetSendBuffAddr<cs_packet_player_pos>();
		packet->header.type = CS_TYPE_PLAYER_POS;
		packet->x = mPickPos.x;
		packet->y = mPickPos.z;
		
		std::string str;
		str += std::to_string(mPickPos.x) + ", " + std::to_string(mPickPos.y);

		SKCONSOLE << str;

		CONNECT.SendPacket(sizeof(cs_packet_player_pos));

		mPickCal = false;
	}

	//cs_packet_move

	while (CONNECT.ThereIsProcessPacket())
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
	}
	
	unsigned int lDir = 0;

	if(mKey['a'])
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

	if (0 != lDir)
	{
		cs_packet_move *movePacket = CONNECT.GetSendBuffAddr<cs_packet_move>();
		movePacket->header.type = CS_TYPE_MOVE;
		movePacket->moveDir = lDir;

		CONNECT.SendPacket(sizeof(cs_packet_move));
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
}
void SceneMMO::KeyBoardUp(unsigned char key, int x, int y)
{
	mKey[key] = false;
}
