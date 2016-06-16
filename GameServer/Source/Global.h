#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "Common.h"

#define DEBUG 0
#define GLOBAL Global::GetInstance()

class Global
{
public:
	static Global& GetInstance()
	{
		static Global instance;
		return instance;
	}

public:
	HANDLE mhIocp;
	bool mShutdown;

private:
	Global(const Global& tmp) {}
	Global() 
	{
		mhIocp = 0;
		mShutdown = false;
	}
};


#endif 
