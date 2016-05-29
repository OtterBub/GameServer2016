#include "ConnectSystem.h"
#include "../Global.h"
#include "../System/ClientMgr.h"

void ConnectSystem::ConnectInitialize()
{
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	GLOBAL.mhIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	GLOBAL.mShutdown = false;
}

void ConnectSystem::AcceptThread()
{
	sockaddr_in listen_addr;

	SOCKET accept_socket = WSASocket( AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED );

	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(ADDR_ANY);
	listen_addr.sin_port = htons( SERVER_PORT );

	//bind
	::bind( accept_socket, reinterpret_cast<sockaddr*>(&listen_addr), sizeof(listen_addr) );
	listen( accept_socket, 10 );

	// g_isShutdown
	while (false == GLOBAL.mShutdown)
	{
		sockaddr_in client_addr;
		int addr_size = sizeof(client_addr);
		SOCKET new_client = WSAAccept(accept_socket,
			reinterpret_cast<sockaddr*>(&client_addr), &addr_size, NULL, NULL );
		
		if (INVALID_SOCKET == new_client)
		{
			//error process
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

		// put player message

		CLIENT(new_id).is_connected = true;

		DWORD flags = 0;
		int ret = WSARecv( new_client, &CLIENT(new_id).recv_overlap.wsabuf/* wsabuf */, 1, NULL,
			&flags, &CLIENT(new_id).recv_overlap.OriginalOverlap/*original_overlap*/, NULL);

		if (0 != ret)
		{
			if (WSA_IO_PENDING != WSAGetLastError())
			{
				// error process
			}
		}
	}
}

void ConnectSystem::WorkerThread()
{
	static int count = 0;
	ConnectSystem lConnect;

	lConnect.mOverlap.operation = count++;
	lConnect.TestFunc();
}
