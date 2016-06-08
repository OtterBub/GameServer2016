#include "Connect.h"
#include "..\Global.h"
#include "..\System\MGR\ClientMgr.h"

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

		CLIENT(new_id).info.mPos.x = 0;
		CLIENT(new_id).info.mPos.y = 0;

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_client), GLOBAL.mhIocp, new_id, 0);
		std::cout << "Connect User id: " << new_id << std::endl;

		// put player message
		sc_packet_player_pos playerPosPacket;
		playerPosPacket.header.size = sizeof(playerPosPacket);
		playerPosPacket.header.type = SC_TYPE_MOVE;
		playerPosPacket.x = 0;
		playerPosPacket.y = 0;
		Connect::SendPacket( reinterpret_cast<unsigned char*>(&playerPosPacket), new_id );

		playerPosPacket.x = 110;
		playerPosPacket.y = 110;
		Connect::SendPacket(reinterpret_cast<unsigned char*>(&playerPosPacket), new_id);

		playerPosPacket.x = 220;
		playerPosPacket.y = 220;
		Connect::SendPacket(reinterpret_cast<unsigned char*>(&playerPosPacket), new_id);

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
			CLIENTMGR.DeleteClient(key);
			std::cout << "clientNum: " << key << " logout:: " << std::endl;
			continue;
		}
#if DEBUG
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
		
		default:
			std::cout << "Unkown IOCP event! [ " << my_overlap->operation << " ]\n";
			break;
		}
	}

}

void Connect::SendPacket(unsigned char *packet, unsigned int key)
{
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

void Connect::ProcessPacket(unsigned char* packet, unsigned int key)
{
	packet_header *header = reinterpret_cast<packet_header*>(packet);

#if DEBUG
	std::cout << "[" << key << "] size: " << (int)header->size
		<< " type: " << (int)header->type << " ";
#endif

	switch ( header->type )
	{
	case CS_TYPE_MOVE:
	{
		cs_packet_move *movePacket = reinterpret_cast<cs_packet_move*>(packet);
#if DEBUG
		std::cout << "TYPE MOVE: " << movePacket->moveDir << std::endl;
#endif
		break;
	}

	default:
		std::cout << "[" << key << "]" << " UnkownPacket \n";
		break;
	}
}