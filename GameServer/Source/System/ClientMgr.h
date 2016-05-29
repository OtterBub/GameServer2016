#ifndef __CLIENT_MGR_H__
#define __CLIENT_MGR_H__
#include "../Thread/Thread.h"
#include "../Object/Player.h"
#include "ConnectSystem.h"


#define CLIENTMGR ClientMgr::GetInstance()
#define CLIENT(x) ClientMgr::GetInstance().GetClient(x)

struct Client
{
	Client()
	{
		s = NULL;
		is_connected = false;
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
};

class ClientMgr
{
public:
	static ClientMgr& GetInstance();
	
	Client& GetClient(unsigned int index);
	bool ExistClient(unsigned int index);
	void DeleteClient(unsigned int index);

private:
	ClientMgr() {}
	ClientMgr(const ClientMgr& tmp) {}

private:
	std::map<unsigned int, Client> mClientList;
};


#endif