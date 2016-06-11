#ifndef __CLIENT_MGR_H__
#define __CLIENT_MGR_H__
#include "../../Common.h"
#include "../../Object/Player.h"
#include "../Connect.h"
#include "MTObjectMgr.h"

//#define CLIENTMGR ClientMgr::GetInstance()
//#define CLIENT(x) ClientMgr::GetInstance().GetClient(x)

struct Client
{
	Client()
	{
		s = NULL;
		is_connected = false;
		recv_overlap.operation = OP_RECV;
		recv_overlap.wsabuf.buf = reinterpret_cast<char*>(recv_overlap.iocp_buff);
		recv_overlap.wsabuf.len = sizeof(recv_overlap.iocp_buff);
		memset(&recv_overlap.OriginalOverlap, 0, sizeof(recv_overlap.OriginalOverlap));
		memset(&packet_buff, 0, sizeof(packet_buff));
		packet_size = 0;
		previous_size = 0;
	}

	SOCKET s;
	bool is_connected;
	std::unordered_set<int> view_list;
	std::mutex viewlist_lock;

	Player info;

	OverlapEx recv_overlap;
	int packet_size;
	int previous_size;
	unsigned char packet_buff[MAX_PACKET_SIZE];

private:
	Client(const Client& tmp)
	{
		std::cout << "copy" << std::endl;
	}
};

class NPC
{
public:
	bool is_active;
};

#define CLIENTMGR MTObjectMGR<unsigned int, Client>::GetInstance()
#define CLIENT(x) MTObjectMGR<unsigned int, Client>::GetInstance().GetObj(x)

#define NPCMGR MTObjectMGR<unsigned int, NPC>::GetInstance()
#define NPC(x) MTObjectMGR<unsigned int, NPC>::GetInstance().GetObj(x)


#endif