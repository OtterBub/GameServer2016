#include "SceneMMO.h"

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
			float lGroundSize = 300;
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

		glPushMatrix();
		{
			//glLoadIdentity();
			glTranslatef( wx, wy, wz );
			glColor4f( 1, 1, 1, 1 );
			glutSolidCube(10);
		}
		glPopMatrix();
	}
	glPopMatrix();
}
void SceneMMO::Update(double dt)
{
}
void SceneMMO::Reshape(int w, int h)
{
}

void SceneMMO::Mouse(int button, int state, int x, int y)
{
	GLdouble projection[16];
	GLint viewPort[4];
	GLint realy;
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

	std::string console;

	console += std::to_string(wx);
	console += ", ";
	console += std::to_string(wy);
	console += ", ";
	console += std::to_string(wz);

	SKCONSOLE << console;

}
void SceneMMO::MouseMotion(int x, int y)
{
}