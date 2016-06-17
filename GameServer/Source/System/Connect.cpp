#include "Connect.h"
#include "..\Global.h"
#include "..\System\MGR\ObjectMgrList.h"
#include "..\System\MGR\DataBaseMgr.h"
#include "../System/MGR/EventMgr.h"

RWLock Connect::connectLock;

void Connect::ConnectInitialize()
{
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	GLOBAL.mhIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	GLOBAL.mShutdown = false;
}

void Connect::AcceptThread()
{
	sockaddr_in listen_addr;

	SOCKET accept_socket = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(ADDR_ANY);
	listen_addr.sin_port = htons(SERVER_PORT);

	//bind
	::bind(accept_socket, reinterpret_cast<sockaddr*>(&listen_addr), sizeof(listen_addr));
	listen(accept_socket, 10);

	// g_isShutdown
	while (false == GLOBAL.mShutdown)
	{
		sockaddr_in client_addr;
		int addr_size = sizeof(client_addr);
		SOCKET new_client = WSAAccept(accept_socket,
			reinterpret_cast<sockaddr*>(&client_addr), &addr_size, NULL, NULL);

		if (INVALID_SOCKET == new_client)
		{
			//error
			//err_display("WSAAccept()");
		}

		int new_id = -1;
		for (auto i = 0; i < MAX_USER; ++i)
		{
			if (false == CLIENTMGR.ExistClient(i))
			{
				new_id = i;
				break;
			}
		}

		if (-1 == new_id)
		{
			std::cout << "MAX user\n";
			closesocket(new_client);
			continue;
		}

		// client initialize
		CLIENT(new_id).s = new_client;
		
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_client), GLOBAL.mhIocp, new_id, 0);
		std::cout << "Connect User id: " << new_id << std::endl;

		// put player message
		CLIENT(new_id).mViewListLock.WriteLock();
		CLIENT(new_id).view_list.clear();
		CLIENT(new_id).mViewListLock.WriteUnLock();

		DWORD flags = 0;
		int ret = WSARecv(new_client, &CLIENT(new_id).recv_overlap.wsabuf/* wsabuf */, 1, NULL,
			&flags, &CLIENT(new_id).recv_overlap.OriginalOverlap/*original_overlap*/, NULL);

		if (0 != ret)
		{
			if (WSA_IO_PENDING != WSAGetLastError())
			{
				// error
				//err_display("WSARecv()");
			}
		}
	}
}

void Connect::DoMove(unsigned int key)
{
	if (NPCMGR.ExistClient(key))
	{
		if (false == NPC(key).is_active) return;
		
		std::unordered_set<unsigned int> view_list;
		for (auto pl = 0; pl < USERMAX; ++pl)
		{
			if (CLIENTMGR.ExistClient(pl))
			{
				if (CLIENT(pl).is_connected)
				{
					Vector2f newClientPos = Vector2f(NPC(key).info.mPos.x, NPC(key).info.mPos.y);
					Vector2f targetClientPos = Vector2f(CLIENT(pl).info.mPos.x, CLIENT(pl).info.mPos.y);
					if (SquareCheck(newClientPos, targetClientPos, VIEWDIST))
					{
						view_list.insert(pl);
					}
				}
			}
		}

		switch (rand() % 4)
		{
		case 0: NPC(key).info.mPos.x++; break;
		case 1: NPC(key).info.mPos.x--; break;
		case 2: NPC(key).info.mPos.y++; break;
		case 3: NPC(key).info.mPos.y--; break;
		default: break;
		}

		std::unordered_set<unsigned int> new_list;
		for (auto pl = 0; pl < USERMAX; ++pl)
		{
			if (CLIENTMGR.ExistClient(pl))
			{
				if (CLIENT(pl).is_connected)
				{
					Vector2f newClientPos = Vector2f(NPC(key).info.mPos.x, NPC(key).info.mPos.y);
					Vector2f targetClientPos = Vector2f(CLIENT(pl).info.mPos.x, CLIENT(pl).info.mPos.y);
					if (SquareCheck(newClientPos, targetClientPos, VIEWDIST))
					{
						new_list.insert(pl);
					}
				}
			}
		}

		for (auto pl : view_list)
		{
			if (0 == new_list.count(pl))
			{
				CLIENT(pl).mViewListLock.WriteLock();
				CLIENT(pl).view_list.erase(key);
				CLIENT(pl).mViewListLock.WriteUnLock();

				sc_packet_remove_object removePacket;
				removePacket.id = key;
				removePacket.objType = TYPE_MONSTER;
				Connect::SendPacket(&removePacket, pl);
			}
			else
			{
				sc_packet_position_info posInfo;
				posInfo.id = key;
				posInfo.objType = TYPE_MONSTER;
				posInfo.x_pos = NPC(key).info.mPos.x;
				posInfo.y_pos = NPC(key).info.mPos.y;

				Connect::SendPacket(&posInfo, pl);
			}
		}

		for (auto pl : new_list)
		{
			if (0 != view_list.count(pl)) continue;
			sc_packet_add_object addPacket;
			addPacket.id = key;
			addPacket.objType = TYPE_MONSTER;
			addPacket.x_pos = NPC(key).info.mPos.x;
			addPacket.y_pos = NPC(key).info.mPos.y;
			Connect::SendPacket(&addPacket, pl);
		}
		int now = GetTickCount();
		/*if (key > 0 && key < 100)
			std::cout << "move time" << (now - NPC(key).last_move_time) << std::endl;*/
		NPC(key).last_move_time = now;

		TIMERMGR.PushEvent(key, GetTickCount() + 1000, OP_MOVE_AI);

		if (true == new_list.empty()) NPC(key).is_active = false;
	}
}

void Connect::WorkerThread()
{
	//Connect lConnect;

	while (false == GLOBAL.mShutdown)
	{
		DWORD iosize;
		DWORD key;
		OverlapEx *my_overlap;
		BOOL result = GetQueuedCompletionStatus(GLOBAL.mhIocp, &iosize, &key, reinterpret_cast<LPOVERLAPPED*>(&my_overlap), INFINITE);

		if (FALSE == result)
		{
			// error
			if (WSA_IO_PENDING != WSAGetLastError())
			{
				//err_display("GetQueuedCompletionStatus()");
			}
		}

		// disconnect
		if (0 == iosize)
		{
			closesocket(CLIENT(key).s);
			CLIENT(key).is_connected = false;

			connectLock.WriteLock();
			std::string query = "UPDATE dbo.user_table SET ";
			query += "XPos = " + std::to_string(CLIENT(key).info.mPos.x) + ", ";
			query += "YPos = " + std::to_string(CLIENT(key).info.mPos.y) + ", ";
			query += "CLevel = " + std::to_string(CLIENT(key).info.level) + ", ";
			query += "EXP = " + std::to_string(CLIENT(key).info.exp) + ", ";
			query += "Attack = " + std::to_string(CLIENT(key).info.atk) + ", ";
			query += "Defence = " + std::to_string(CLIENT(key).info.def) + ", ";
			query += "HP = " + std::to_string(CLIENT(key).info.hp) + " ";
			query += "WHERE ID = " + std::to_string(CLIENT(key).info.id);
			std::cout << query << std::endl;
			DBMGR.Query(query);

			CLIENTMGR.DeleteClient(key);
			connectLock.WriteUnLock();

			sc_packet_remove_object removePacket;
			removePacket.header.size = sizeof(removePacket);
			removePacket.header.type = SC_REMOVE_OBJECT;
			removePacket.id = key;
			removePacket.objType = TYPE_PLAYER;
			SendBroadCasting(&removePacket);
			
			std::cout << "clientNum: " << key << " logout:: " << std::endl;
			continue;
		}
#if DEBUG
		if( OP_TEST != my_overlap->operation )
			std::cout << "clientNum: " << key << " operation:: " << my_overlap->operation << std::endl;
#endif
		
		switch (my_overlap->operation)
		{
		case OP_RECV:
		{
			unsigned char *buf_ptr = CLIENT(key).recv_overlap.iocp_buff;
			int remained = iosize;
			while (0 < remained)
			{
				if (0 == CLIENT(key).packet_size)
				{
					CLIENT(key).packet_size = buf_ptr[0];
				}

				int required = CLIENT(key).packet_size =
					CLIENT(key).packet_size - CLIENT(key).previous_size;

				if (remained >= required)
				{
					memcpy(CLIENT(key).packet_buff + CLIENT(key).previous_size,
						buf_ptr, required);

					// ProcessPacket
					ProcessPacket(reinterpret_cast<unsigned char*>(CLIENT(key).packet_buff), key);

					buf_ptr += required;
					remained -= required;
					CLIENT(key).packet_size = 0;
					CLIENT(key).previous_size = 0;
				}
				else
				{
					memcpy(CLIENT(key).packet_buff + CLIENT(key).previous_size,
						buf_ptr, remained);
					buf_ptr += remained;
					CLIENT(key).previous_size += remained;
					remained = 0;
				}
				DWORD flags = 0;
				int ret = WSARecv(CLIENT(key).s, &CLIENT(key).recv_overlap.wsabuf, 1, NULL, &flags, 
					&CLIENT(key).recv_overlap.OriginalOverlap, NULL);
				if (0 != ret)
				{
					if (WSA_IO_PENDING != WSAGetLastError())
					{
						//err_display("WorkerThread::Recv()");
					}
				}
			}
			break;
		}
		case OP_SEND:
		{
			delete my_overlap;
			break;
		}
		case OP_MOVE_AI:
		{
			Connect::DoMove(key);
			delete my_overlap;
			break;
		}
		case OP_TEST:
		{
			std::cout << "opTest" << std::endl;

			//Connect::SendPacket();
			delete my_overlap;
			break;
		}
		default:
			std::cout << "Unkown IOCP event! [ " << my_overlap->operation << " ]\n";
			break;
		}
	}

}

void Connect::SendPacket(unsigned char *dataPtr, unsigned int key)
{
	if (CLIENTMGR.ExistClient(key) == false) return;

	/*unsigned char* packet = reinterpret_cast<unsigned char*>(dataPtr);
	packet_header *header = reinterpret_cast<packet_header*>(packet);*/
	OverlapEx *over = new OverlapEx;
	memset(over, 0, sizeof(OverlapEx));

	over->operation = OP_SEND;
	over->wsabuf.buf = reinterpret_cast<CHAR*>(over->iocp_buff);
	over->wsabuf.len = dataPtr[0];

	//std::cout << "type " << (unsigned int)header->type << std::endl;

	memcpy(over->iocp_buff, dataPtr, dataPtr[0]);

	int ret = WSASend(CLIENT(key).s, &over->wsabuf, 1, NULL, 0,
		&over->OriginalOverlap, NULL);

	if (0 != ret)
	{
		int errorcode = WSAGetLastError();
		if ((errorcode != 10054) && (errorcode != 10038))
			std::cout << "WSASend ErrorCode: " << errorcode << std::endl;
	}
}

void Connect::SendPacket(void *dataPtr, unsigned int key)
{
	if (CLIENTMGR.ExistClient(key) == false) return;

	unsigned char* packet = reinterpret_cast<unsigned char*>(dataPtr);
	packet_header *header = reinterpret_cast<packet_header*>(packet);
	OverlapEx *over = new OverlapEx;
	memset(over, 0, sizeof(OverlapEx));

	over->operation = OP_SEND;
	over->wsabuf.buf = reinterpret_cast<CHAR*>(over->iocp_buff);
	over->wsabuf.len = packet[0];

	//std::cout << "type " << (unsigned int)header->type << std::endl;

	memcpy(over->iocp_buff, packet, packet[0]);

	int ret = WSASend(CLIENT(key).s, &over->wsabuf, 1, NULL, 0,
		&over->OriginalOverlap, NULL);

	if (0 != ret)
	{
		int errorcode = WSAGetLastError();
		std::cout << "WSASend ErrorCode: " << errorcode << std::endl;
	}
}

void Connect::SendPacket(void *packet, unsigned int size, unsigned int type, unsigned int key)
{
	if (CLIENTMGR.ExistClient(key) == false) return;

	char* data = reinterpret_cast<char*>(packet);
	packet_header *header = reinterpret_cast<packet_header*>(data);
	header->size = size;
	header->type = type;
	OverlapEx *over = new OverlapEx;

	memset(over, 0, sizeof(OverlapEx));
	over->operation = OP_SEND;
	over->wsabuf.buf = reinterpret_cast<CHAR*>(over->iocp_buff);
	over->wsabuf.len = size;

	//std::cout << "type " << (unsigned int)header->type << std::endl;

	memcpy(over->iocp_buff, data, size);

	int ret = WSASend(CLIENT(key).s, &over->wsabuf, 1, NULL, 0,
		&over->OriginalOverlap, NULL);

	if (0 != ret)
	{
		int errorcode = WSAGetLastError();
		std::cout << "WSASend ErrorCode: " << errorcode << std::endl;
	}
}

void Connect::SendBroadCasting(void* packet, unsigned int except )
{
	std::map<unsigned int, ClientStruct> tempMap = CLIENTMGR.GetList();
	for (auto it = tempMap.begin(); it != tempMap.end(); ++it)
	{
		if (it->first != except)
			Connect::SendPacket(packet, it->first);
	}
}

void Connect::ProcessPacket(unsigned char* packet, unsigned int key)
{
	packet_header *header = reinterpret_cast<packet_header*>(packet);

#if DEBUG
	std::cout << "[" << key << "] size: " << (int)header->size
		<< " type: " << (int)header->type << " ";
#endif

	switch ( header->type )
	{
	case CS_LOGIN:
	{
		cs_packet_login *loginPacket = reinterpret_cast<cs_packet_login*>(packet);
		char nick[100] = "gamepsk";
		size_t i;
		wcstombs_s(&i, nick, 100, loginPacket->nick, 100);
		DBMGR.SearchNick( nick, key );

		// put player message
		CLIENT(key).mViewListLock.WriteLock();
		CLIENT(key).view_list.clear();
		CLIENT(key).mViewListLock.WriteUnLock();

		sc_packet_login_ok loginOkPacket;

		loginOkPacket.header.size = sizeof(sc_packet_login_ok);
		loginOkPacket.header.type = SC_LOGIN_OK;
		loginOkPacket.id = key;
		loginOkPacket.x_pos = CLIENT(key).info.mPos.x;
		loginOkPacket.y_pos = CLIENT(key).info.mPos.y;
		loginOkPacket.HP = CLIENT(key).info.hp;
		loginOkPacket.EXP = CLIENT(key).info.exp;
		Connect::SendPacket(&loginOkPacket, key);

		std::cout << "sizeof: " << sizeof(sc_packet_login_ok) << "\n";
		

		sc_packet_add_object addPacket;
		addPacket.id = key;
		addPacket.x_pos = CLIENT(key).info.mPos.x;
		addPacket.y_pos = CLIENT(key).info.mPos.y;
		addPacket.objType = TYPE_PLAYER;
		Connect::SendPacket(&addPacket, key);

		for (auto i = 0; i < USERMAX; ++i)
		{
			if (i == key) continue;
			if (CLIENTMGR.ExistClient(i))
			{
				if (CLIENT(i).is_connected)
				{
					Vector2f newClientPos = Vector2f(CLIENT(key).info.mPos.x, CLIENT(key).info.mPos.y);
					Vector2f targetClientPos = Vector2f(CLIENT(i).info.mPos.x, CLIENT(i).info.mPos.y);
					if (SquareCheck(newClientPos, targetClientPos, VIEWDIST))
					{
						CLIENT(i).mViewListLock.WriteLock();
						CLIENT(i).view_list.insert(key);
						CLIENT(i).mViewListLock.WriteUnLock();
						Connect::SendPacket(&addPacket, i);
					}
				}
			}
		}

		for (auto i = 0; i < USERMAX; ++i)
		{
			if (i == key) continue;
			if (CLIENTMGR.ExistClient(i))
			{
				if (CLIENT(i).is_connected)
				{
					Vector2f newClientPos = Vector2f(CLIENT(key).info.mPos.x, CLIENT(key).info.mPos.y);
					Vector2f targetClientPos = Vector2f(CLIENT(i).info.mPos.x, CLIENT(i).info.mPos.y);
					if (SquareCheck(newClientPos, targetClientPos, VIEWDIST))
					{
						CLIENT(key).mViewListLock.WriteLock();
						CLIENT(key).view_list.insert(key);
						CLIENT(key).mViewListLock.WriteUnLock();

						addPacket.id = i;
						addPacket.x_pos = CLIENT(i).info.mPos.x;
						addPacket.y_pos = CLIENT(i).info.mPos.y;
						Connect::SendPacket(&addPacket, key);
					}
				}
			}
		}

		for (auto i = 0; i < NPCMAX; ++i)
		{
			if (NPCMGR.ExistClient(i))
			{
				if (NPC(i).is_active)
				{
					Vector2f newClientPos = Vector2f(CLIENT(key).info.mPos.x, CLIENT(key).info.mPos.y);
					Vector2f targetClientPos = Vector2f(NPC(i).info.mPos.x, NPC(i).info.mPos.y);
					if (SquareCheck(newClientPos, targetClientPos, VIEWDIST))
					{
						if (!NPC(i).is_active)
						{
							NPC(i).is_active = true;
							TIMERMGR.PushEvent(i, GetTickCount() + 1000, OP_MOVE_AI);
						}

						CLIENT(key).mViewListLock.WriteLock();
						CLIENT(key).npc_view_list.insert(i);
						CLIENT(key).mViewListLock.WriteUnLock();

						addPacket.id = i;
						addPacket.x_pos = NPC(i).info.mPos.x;
						addPacket.y_pos = NPC(i).info.mPos.y;
						addPacket.objType = TYPE_MONSTER;
						Connect::SendPacket(&addPacket, key);
					}
				}
			}
		}
		CLIENT(key).is_connected = true;
		break;
	}
	case CS_LOGOUT:
	{
		std::string query = "UPDATE dbo.user_table SET ";
		query += "XPos = " + std::to_string(CLIENT(key).info.mPos.x) + ", ";
		query += "YPos = " + std::to_string(CLIENT(key).info.mPos.y) + ", ";
		query += "CLevel = " + std::to_string(CLIENT(key).info.level) + ", ";
		query += "EXP = " + std::to_string(CLIENT(key).info.exp) + ", ";
		query += "Attack = " + std::to_string(CLIENT(key).info.atk) + ", ";
		query += "Defence = " + std::to_string(CLIENT(key).info.def) + ", ";
		query += "HP = " + std::to_string(CLIENT(key).info.hp) + " ";
		query += "WHERE ID = " + std::to_string(CLIENT(key).info.id);
		
		DBMGR.Query(query);

		std::cout << "logout " << key << std::endl;
		break;
	}
	case CS_MOVE:
	{
		cs_packet_move *movePacket = reinterpret_cast<cs_packet_move*>(packet);
#if DEBUG
		std::cout << "TYPE MOVE: " << movePacket->moveDir << std::endl;
#endif
		static float speed = 1;
		CLIENT(key).info.mPos.x += (movePacket->moveDir & moveDir::MOVE_RIGHT) * speed;
		CLIENT(key).info.mPos.x += ((movePacket->moveDir & moveDir::MOVE_LEFT) >> 1)  * -speed;
		CLIENT(key).info.mPos.y += ((movePacket->moveDir & moveDir::MOVE_UP) >> 2)  * -speed;
		CLIENT(key).info.mPos.y += ((movePacket->moveDir & moveDir::MOVE_DOWN) >> 3) * speed;

		Vector3f pos = CLIENT(key).info.mPos;

		sc_packet_position_info playerPosPacket;

		playerPosPacket.objType = TYPE_PLAYER;
		playerPosPacket.x_pos = pos.x;
		playerPosPacket.y_pos = pos.y;
		playerPosPacket.id = key;
		Connect::SendPacket(&playerPosPacket, key);

		// 리스트에 들어온다.
		std::unordered_set<unsigned int> new_list;
		for (auto i = 0; i < USERMAX; ++i)
		{
			if (i == key) continue;
			if (CLIENTMGR.ExistClient(i))
			{
				if (CLIENT(i).is_connected)
				{
					Vector2f newClientPos = Vector2f(CLIENT(key).info.mPos.x, CLIENT(key).info.mPos.y);
					Vector2f targetClientPos = Vector2f(CLIENT(i).info.mPos.x, CLIENT(i).info.mPos.y);
					if (SquareCheck(newClientPos, targetClientPos, VIEWDIST))
					{
						new_list.insert(i);
					}
				}
			}
		}

		for (auto i : new_list)
		{
			CLIENT(key).mViewListLock.WriteLock();
			if (0 == CLIENT(key).view_list.count(i))
			{
				CLIENT(key).view_list.insert(i);
				CLIENT(key).mViewListLock.WriteUnLock();

				sc_packet_add_object addPacket;
				addPacket.id = i;
				addPacket.objType = TYPE_PLAYER;
				addPacket.x_pos = CLIENT(i).info.mPos.x;
				addPacket.y_pos = CLIENT(i).info.mPos.y;

				Connect::SendPacket(&addPacket, key);

				CLIENT(i).mViewListLock.WriteLock();
				if (0 == CLIENT(i).view_list.count(key))
				{
					CLIENT(i).view_list.insert(key);
					CLIENT(i).mViewListLock.WriteUnLock();

					sc_packet_add_object addPacket;
					addPacket.id = key;
					addPacket.objType = TYPE_PLAYER;
					addPacket.x_pos = CLIENT(key).info.mPos.x;
					addPacket.y_pos = CLIENT(key).info.mPos.y;

					Connect::SendPacket(&addPacket, i);
				}
				else
				{
					CLIENT(i).mViewListLock.WriteUnLock();
					Connect::SendPacket(&playerPosPacket, i);
				}
			}
			else
			{
				CLIENT(key).mViewListLock.WriteUnLock();
				// is npc continue;

				CLIENT(i).mViewListLock.WriteLock();
				if (1 >= CLIENT(i).view_list.count(key))
				{
					CLIENT(i).mViewListLock.WriteUnLock();
					Connect::SendPacket(&playerPosPacket, i);
				}
				else
				{
					CLIENT(i).view_list.insert(key);
					CLIENT(i).mViewListLock.WriteUnLock();
					
					sc_packet_add_object addPacket;
					addPacket.id = key;
					addPacket.objType = TYPE_PLAYER;
					addPacket.x_pos = CLIENT(key).info.mPos.x;
					addPacket.y_pos = CLIENT(key).info.mPos.y;
					
					Connect::SendPacket(&addPacket, i);
				}
			}
		}

		std::unordered_set<unsigned int> npc_new_list;
		for (auto i = 0; i < NPCMAX; ++i)
		{
			if (NPCMGR.ExistClient(i))
			{
				Vector2f newClientPos = Vector2f(CLIENT(key).info.mPos.x, CLIENT(key).info.mPos.y);
				Vector2f targetClientPos = Vector2f(NPC(i).info.mPos.x, NPC(i).info.mPos.y);
				if (SquareCheck(newClientPos, targetClientPos, VIEWDIST))
				{
					npc_new_list.insert(i);
				}
			}
		}

		for (auto i : npc_new_list)
		{
			CLIENT(key).mViewListLock.WriteLock();
			if (0 == CLIENT(key).npc_view_list.count(i))
			{
				CLIENT(key).npc_view_list.insert(i);
				CLIENT(key).mViewListLock.WriteUnLock();

				sc_packet_add_object npcAddPacket;
				npcAddPacket.id = i;
				npcAddPacket.objType = TYPE_MONSTER;
				npcAddPacket.x_pos = NPC(i).info.mPos.x;
				npcAddPacket.y_pos = NPC(i).info.mPos.y;

				Connect::SendPacket(&npcAddPacket, key);

				if (!NPC(i).is_active)
				{
					NPC(i).is_active = true;
					TIMERMGR.PushEvent(i, GetTickCount() + 1000, OP_MOVE_AI);
				}
				
				// add Timer
			}
			else
			{
				CLIENT(key).mViewListLock.WriteUnLock();
				// is npc continue;
			}
		}

		// 리스트에서 나간다.
		std::vector<unsigned int> remove_list;
		CLIENT(key).mViewListLock.WriteLock();
		for (auto i : CLIENT(key).view_list)
		{
			if (key == i) continue;
			if (0 == new_list.count(i))
			{
				remove_list.push_back(i);
			}
		}
		for (auto i : remove_list)
		{
			CLIENT(key).view_list.erase(i);
		}

		std::vector<unsigned int> npc_remove_list;
		for (auto i : CLIENT(key).npc_view_list)
		{
			if (0 == npc_new_list.count(i))
			{
				npc_remove_list.push_back(i);
			}
		}
		for (auto i : npc_remove_list)
		{
			CLIENT(key).npc_view_list.erase(i);
		}
		CLIENT(key).mViewListLock.WriteUnLock();

		for (auto i : remove_list)
		{
			sc_packet_remove_object removePacket;
			removePacket.id = i;
			removePacket.objType = TYPE_PLAYER;
			Connect::SendPacket(&removePacket, key);
		}

		for (auto i : npc_remove_list)
		{
			sc_packet_remove_object removePacket;
			removePacket.header.size = sizeof(sc_packet_remove_object);
			removePacket.id = i;
			removePacket.objType = TYPE_MONSTER;
			Connect::SendPacket(&removePacket, key);
		}

		for (auto i : remove_list)
		{
			// if npc continue;
			CLIENT(i).mViewListLock.WriteLock();
			if (0 != CLIENT(i).view_list.count(key))
			{
				CLIENT(i).view_list.erase(key);
				CLIENT(i).mViewListLock.WriteUnLock();
				
				sc_packet_remove_object removePacket;
				removePacket.id = key;
				removePacket.objType = TYPE_PLAYER;
				Connect::SendPacket(&removePacket, i);
			}
			else CLIENT(i).mViewListLock.WriteUnLock();
		}

		break;
	}
	case CS_ATTACK:
	{
		CLIENT(key).mViewListLock.WriteLock();

		for (auto i : CLIENT(key).npc_view_list)
		{
			if (NPCMGR.ExistClient(i))
			{
				Vector2f newClientPos = Vector2f(CLIENT(key).info.mPos.x, CLIENT(key).info.mPos.y);
				Vector2f targetClientPos = Vector2f(NPC(i).info.mPos.x, NPC(i).info.mPos.y);
				if (SquareCheck(newClientPos, targetClientPos, ATKDIST))
				{
					NPC(i).info.hp -= CLIENT(key).info.atk;

					sc_packet_attack_object attackPacket;
					attackPacket.attacker_id = key;
					attackPacket.attacker_type = TYPE_PLAYER;
					//wcscpy_s(attackPacket.attacker_nick, 10, CLIENT(key).info.char_id);
					attackPacket.attack_to_id = i;
					attackPacket.attack_to_type = TYPE_MONSTER;
					//wcscpy_s(attackPacket.attacker_to_nick, 10, NPC(i).info.char_id);
					attackPacket.attack_to_hp = NPC(i).info.hp;

					Connect::SendPacket(&attackPacket, key);
					
					for (auto pl : CLIENT(key).view_list)
					{
						Connect::SendPacket(&attackPacket, pl);
					}

					
					if (NPC(i).info.hp <= 0)
					{
						NPCMGR.DeleteClient(i);
						sc_packet_remove_object removePacket;
						removePacket.id = i;
						removePacket.objType = TYPE_MONSTER;

						Connect::SendPacket(&removePacket, key);

						for (auto pl : CLIENT(key).view_list)
						{
							Connect::SendPacket(&removePacket, pl);
						}
					}
				}
			}
		}
		std::cout << '[' << key << ']' << "CS_ATTACK\n";
		CLIENT(key).mViewListLock.WriteUnLock();
		break;
	}
	default:
		std::cout << "[" << key << "]" << " UnkownPacket \n";
		break;
	}
}