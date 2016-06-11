#pragma once
#ifndef __NPC_MGR_H__
#define __NPC_MGR_H__

#include "MTObjectMgr.h"

class NPC
{
public:
	bool is_active;
};

#define NPCMGR MTObjectMGR<unsigned int, NPC>::GetInstance()
#define NPCMGRGETINDEX(x) MTObjectMGR<unsigned int, NPC>::GetInstance()

#define NPC(x) MTObjectMGR<unsigned int, NPC>::GetInstance().GetObj(x)
#define NPCINDEX(x, y) MTObjectMGR<unsigned int, NPC>::GetInstance( y ).GetObj(x)


#endif