#include <stdio.h>
#include "System\DrawSystem.h"

int main(int argc, char *argv[])
{
	glutInit( &argc, argv );
	DrawSystem sys;
	sys.Init();
	sys.Run();

	return 0;
}