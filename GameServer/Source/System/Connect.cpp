#include "Connect.h"
#include "..\Global.h"
#include "..\System\MGR\ObjectMgrList.h"

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

		CLIENT(new_id).info.mPos.x = 50;
		CLIENT(new_id).info.mPos.y = 70;

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_client), GLOBAL.mhIocp, new_id, 0);
		std::cout << "Connect User id: " << new_id << std::endl;

		// put player message

		CLIENT(new_id).viewlist_lock.lock();
		CLIENT(new_id).view_list.clear();
		CLIENT(new_id).viewlist_lock.unlock();

		sc_packet_login_ok loginPacket;

		/*loginPacket.header.size = sizeof(sc_packet_login_ok);
		loginPacket.header.type = SC_LOGIN_OK;*/
		loginPacket.id = new_id;
		loginPacket.x_pos = CLIENT(new_id).info.mPos.x;
		loginPacket.y_pos = CLIENT(new_id).info.mPos.y;
		loginPacket.HP = 100;
		loginPacket.EXP = 10;
		Connect::SendPacket( &loginPacket, new_id );

		sc_packet_add_object addPacket;
		addPacket.id = new_id;
		addPacket.x_pos = CLIENT(new_id).info.mPos.x;
		addPacket.y_pos = CLIENT(new_id).info.mPos.y;
		addPacket.objType = TYPE_PLAYER;

		for (auto i = 0; i < USERMAX; ++i)
		{
			if (i == new_id) continue;
			if (CLIENTMGR.ExistClient(i))
			{
				if (CLIENT(i).is_connected)
				{
					Vector2f newClientPos = Vector2f(CLIENT(new_id).info.mPos.x, CLIENT(new_id).info.mPos.y);
					Vector2f targetClientPos = Vector2f(CLIENT(i).info.mPos.x, CLIENT(i).info.mPos.y);
					if (SquareCheck(newClientPos, targetClientPos, VIEWDIST))
					{
						CLIENT(i).viewlist_lock.lock();
						CLIENT(i).view_list.insert(new_id);
						CLIENT(i).viewlist_lock.unlock();
						Connect::SendPacket(&addPacket, i);
					}
				}
			}
		}

		for (auto i = 0; i < USERMAX; ++i)
		{
			if (i == new_id) continue;
			if (CLIENTMGR.ExistClient(i))
			{
				if (CLIENT(i).is_connected)
				{
					Vector2f newClientPos = Vector2f(CLIENT(new_id).info.mPos.x, CLIENT(new_id).info.mPos.y);
					Vector2f targetClientPos = Vector2f(CLIENT(i).info.mPos.x, CLIENT(i).info.mPos.y);
					if (SquareCheck(newClientPos, targetClientPos, VIEWDIST))
					{
						CLIENT(new_id).viewlist_lock.lock();
						CLIENT(new_id).view_list.insert(new_id);
						CLIENT(new_id).viewlist_lock.unlock();

						addPacket.id = i;
						addPacket.x_pos = CLIENT(i).info.mPos.x;
						addPacket.y_pos = CLIENT(i).info.mPos.y;
						Connect::SendPacket(&addPacket, new_id);
					}
				}
			}
		}
		CLIENT(new_id).is_connected = true;


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

			//connectLock.WriteLock();
			CLIENTMGR.DeleteClient(key);

			sc_packet_player_remove removePacket;
			removePacket.header.size = sizeof(removePacket);
			removePacket.header.type = SC_TYPE_PLAYER_REMOVE;
			removePacket.id = key;
			SendBroadCasting(&removePacket);
			//connectLock.WriteUnLock();
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

void Connect::SendPacket(void *dataPtr, unsigned int key)
{
	if (CLIENTMGR.ExistClient(key) == false) return;

	unsigned char* packet = reinterpret_cast<unsigned char*>(dataPtr);
	packet_header *header = reinterpret_cast<packet_header*>(packet);
	OverlapEx *over = new OverlapEx;

	memset(over, 0, sizeof(OverlapEx));
	over->operation = OP_SEND;
	over->wsabuf.buf = reinterpret_cast<CHAR*>(over->iocp_buff);
	over->wsabuf.len = header->size;

	memcpy(over->iocp_buff, packet, header->size);

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
			CLIENT(key).viewlist_lock.lock();
			if (0 == CLIENT(key).view_list.count(i))
			{
				CLIENT(key).view_list.insert(i);
				CLIENT(key).viewlist_lock.unlock();

				sc_packet_add_object addPacket;
				addPacket.id = i;
				addPacket.objType = TYPE_PLAYER;
				addPacket.x_pos = CLIENT(i).info.mPos.x;
				addPacket.y_pos = CLIENT(i).info.mPos.y;

				Connect::SendPacket(&addPacket, key);

				CLIENT(i).viewlist_lock.lock();
				if (0 == CLIENT(i).view_list.count(key))
				{
					CLIENT(i).view_list.insert(key);
					CLIENT(i).viewlist_lock.unlock();

					sc_packet_add_object addPacket;
					addPacket.id = key;
					addPacket.objType = TYPE_PLAYER;
					addPacket.x_pos = CLIENT(key).info.mPos.x;
					addPacket.y_pos = CLIENT(key).info.mPos.y;

					Connect::SendPacket(&addPacket, i);
				}
				else
				{
					CLIENT(i).viewlist_lock.unlock();
					Connect::SendPacket(&playerPosPacket, i);
				}
			}
			else
			{
				CLIENT(key).viewlist_lock.unlock();
				// is npc continue;

				CLIENT(i).viewlist_lock.lock();
				if (1 >= CLIENT(i).view_list.count(key))
				{
					CLIENT(i).viewlist_lock.unlock();
					Connect::SendPacket(&playerPosPacket, i);
				}
				else
				{
					CLIENT(i).view_list.insert(key);
					CLIENT(i).viewlist_lock.unlock();
					
					sc_packet_add_object addPacket;
					addPacket.id = key;
					addPacket.objType = TYPE_PLAYER;
					addPacket.x_pos = CLIENT(key).info.mPos.x;
					addPacket.y_pos = CLIENT(key).info.mPos.y;
					
					Connect::SendPacket(&addPacket, i);
				}
			}
		}

		// 리스트에서 나간다.
		std::vector<unsigned int> remove_list;
		CLIENT(key).viewlist_lock.lock();
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
		CLIENT(key).viewlist_lock.unlock();

		for (auto i : remove_list)
		{
			sc_packet_remove_object removePacket;
			removePacket.id = i;
			removePacket.objType = TYPE_PLAYER;
			Connect::SendPacket(&removePacket, key);
		}

		for (auto i : remove_list)
		{
			// if npc continue;
			CLIENT(i).viewlist_lock.lock();
			if (0 != CLIENT(i).view_list.count(key))
			{
				CLIENT(i).view_list.erase(key);
				CLIENT(i).viewlist_lock.unlock();
				
				sc_packet_remove_object removePacket;
				removePacket.id = key;
				removePacket.objType = TYPE_PLAYER;
				Connect::SendPacket(&removePacket, i);
			}
			else CLIENT(i).viewlist_lock.unlock();
		}

		break;
	}
	default:
		std::cout << "[" << key << "]" << " UnkownPacket \n";
		break;
	}
}