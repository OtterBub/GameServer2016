#include "ClientConnect.h"
#include "../ClientFrame/System/Scene/SceneMMO.h"

void ClientConnect::ReadPacket()
{
	DWORD iobyte = 0, ioflag = 0;
	while (false == CONNECT.mShutDown)
	{
		int ret = WSARecv(CONNECT.mSocket, &CONNECT.mRecvWSAbuf, 1, &iobyte, &ioflag, NULL, NULL);
		
		if (ret)
		{
			int err_code = WSAGetLastError();
			
			if (WSAEWOULDBLOCK == err_code)
				continue;

			std::cout << "Recv Error [" << err_code << "]\n";
		}

		packet_header *header = reinterpret_cast<packet_header*>(CONNECT.mRecvBuffer);
		BYTE *ptr = reinterpret_cast<BYTE*>(CONNECT.mRecvBuffer);

		while (0 != iobyte)
		{
			if (0 == CONNECT.mInPacketSize) CONNECT.mInPacketSize = ptr[0];
			//if (0 == CONNECT.mInPacketSize) CONNECT.mInPacketSize = ptr[0];
			if (iobyte + CONNECT.mSavedPacketSize >= CONNECT.mInPacketSize)
			{
				memcpy(CONNECT.mPacketBuffer + CONNECT.mSavedPacketSize, ptr, CONNECT.mInPacketSize - CONNECT.mSavedPacketSize);
				// ProcessPacket
				ClientConnect::ProcessPacket(CONNECT.mPacketBuffer);

				ptr += CONNECT.mInPacketSize - CONNECT.mSavedPacketSize;
				iobyte -= CONNECT.mInPacketSize - CONNECT.mSavedPacketSize;
				CONNECT.mInPacketSize = 0;
				CONNECT.mSavedPacketSize = 0;
			}
			else
			{
				memcpy(CONNECT.mPacketBuffer + CONNECT.mSavedPacketSize, ptr, iobyte);
				CONNECT.mSavedPacketSize += iobyte;
				iobyte = 0;
			}
		}
		
		//_sleep(0); // 절대 쉬게해서는 안됌!
	}
}

void ClientConnect::ProcessPacket(char *packet)
{
	//std::cout << "[Recv] type: " << (int)header->type << " size: " << (int)header->size << std::endl;

	/*PacketStore lTempPacket;
	memcpy_s(&lTempPacket, BUF_SIZE, header, BUF_SIZE);
	CONNECT.mDataQueueLock.WriteLock();
	CONNECT.mDataQueue.push(lTempPacket);
	CONNECT.mDataQueueLock.WriteUnLock();*/

	packet_header *header = reinterpret_cast<packet_header*> (packet);

	switch (header->type)
	{
	case SC_LOGIN_OK:
	{
		sc_packet_login_ok *okPacket = reinterpret_cast<sc_packet_login_ok*>(packet);
		CONNECT.mConnectLock.WriteLock();
		PLAYER(okPacket->id).SetColor(Vector4(0, 0, 1, 1));
		PLAYER(okPacket->id).SetPosition(Vector3(okPacket->x_pos, 0, okPacket->y_pos));
		CONNECT.mConnectLock.WriteUnLock();

		std::string str = "LOGIN SUCCESS!! Your Id: " + std::to_string(okPacket->id);
		SKCONSOLE << str;
		break;
	}
	case SC_ADD_OBJECT:
	{
		sc_packet_add_object *addPacket = reinterpret_cast<sc_packet_add_object*>(packet);
		CONNECT.mConnectLock.WriteLock();
		switch (addPacket->objType)
		{
		case TYPE_PLAYER:
		{
			PLAYER(addPacket->id).SetPosition(Vector3(addPacket->x_pos, 0, addPacket->y_pos));
			break;
		}
		case TYPE_MONSTER:
		{
			NPC(addPacket->id).SetPosition(Vector3(addPacket->x_pos, 0, addPacket->y_pos));
			NPC(addPacket->id).SetColor(Vector4(1, 0.3, 0.3, 1));
			break;
		}
		default:
			break;
		}
		CONNECT.mConnectLock.WriteUnLock();
		
		std::string str = "AddObject Id: " + std::to_string(addPacket->id);
		SKCONSOLE << str;
		break;
	}
	case SC_REMOVE_OBJECT:
	{
		sc_packet_remove_object *removePacket = reinterpret_cast<sc_packet_remove_object*>(packet);
		
		CONNECT.mConnectLock.WriteLock();
		switch (removePacket->objType)
		{
		case TYPE_PLAYER:
		{
			PLAYERMGR.DeleteClient(removePacket->id);
			break;
		}
		case TYPE_MONSTER:
		{
			NPCMGR.DeleteClient(removePacket->id);
			break;
		}
		default:
			break;
		}
		CONNECT.mConnectLock.WriteUnLock();

		std::string str = "Remove Id: " + std::to_string(removePacket->id);
		SKCONSOLE << str;
		break;
	}
	case SC_POSITION_INFO:
	{
		sc_packet_position_info *infoPacket = reinterpret_cast<sc_packet_position_info*>(packet);
		switch (infoPacket->objType)
		{
		case TYPE_PLAYER:
		{
			PLAYER(infoPacket->id).SetPosition(Vector3(infoPacket->x_pos, 0, infoPacket->y_pos));
			break;
		}
		case TYPE_MONSTER:
		{
			NPC(infoPacket->id).SetPosition(Vector3(infoPacket->x_pos, 0, infoPacket->y_pos));
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		std::cout << "UnKnown Packet Type: " << (int)header->type << std::endl;
		break;
	}
}

void ClientConnect::Initialize()
{
	int result = 0;

	WSADATA	wsadata;
	result = WSAStartup(MAKEWORD(2, 2), &wsadata);

	mSocket = INVALID_SOCKET;

	if (0 != result)
	{
		std::cout << "Connect Initialize Error Code: " << result << std::endl;
		std::cin >> result;
		exit(1);
	}

	memset(&mSendBuffer, 0, sizeof(mSendBuffer));
	memset(&mRecvBuffer, 0, sizeof(mRecvBuffer));

	mSendWSAbuf.buf = mSendBuffer;
	mSendWSAbuf.len = BUF_SIZE;
	mRecvWSAbuf.buf = mRecvBuffer;
	mRecvWSAbuf.len = BUF_SIZE;

	mInPacketSize = 0;
	mSavedPacketSize = 0;
	mShutDown = false;
}

void ClientConnect::Connect(std::string ipAddr)
{
	struct addrinfo 
		*result = NULL,
		*ptr = NULL,
		hints;
	int iResult = 0;
	char serverport[255] = { 0, };

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP; // TCP Connect

	sprintf_s(serverport, "%d", SERVER_PORT);
	iResult = getaddrinfo(ipAddr.c_str(), serverport, &hints, &result);

	if (0 != iResult)
	{
		std::cout << "getaddrinfo failed error code: " << iResult << std::endl;
		WSACleanup();
		std::cin >> iResult;
		exit(1);
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		mSocket = socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );

		if (INVALID_SOCKET == mSocket)
		{
			std::cout << "socket failed error code: " << WSAGetLastError() << std::endl;
			WSACleanup();
			std::cin >> iResult;
			exit(1);
		}

		iResult = connect( mSocket, ptr->ai_addr, (int)ptr->ai_addrlen );

		if (SOCKET_ERROR == iResult)
		{
			closesocket(mSocket);
			mSocket = INVALID_SOCKET;
			std::cout << "did not connect\n";
		}
	}

	freeaddrinfo( result );

	if (INVALID_SOCKET == mSocket)
	{
		std::cout << "Unable to connect to server!\n";
		WSACleanup();
		std::cin >> iResult;
		exit(1);
	}

	u_long iMode = 1;

	iResult = ioctlsocket( mSocket, FIONBIO, &iMode );
	if (SOCKET_ERROR == iResult)
	{
		std::cout << "ioctlsocket failed error code: " << iResult << std::endl;
		closesocket(mSocket);
		WSACleanup();
		std::cin >> iResult;
		exit(1);
	}

	char value = 1;
	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

	//mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	/*
	SOCKADDR_IN lServerAddr;
	memset(&lServerAddr, 0, sizeof(lServerAddr));
	lServerAddr.sin_family = AF_INET;
	lServerAddr.sin_port = htons(SERVER_PORT);
	lServerAddr.sin_addr.s_addr = inet_addr( ipAddr.c_str() );

	int Result = WSAConnect( mSocket, (sockaddr*)&lServerAddr, sizeof(lServerAddr), NULL, NULL, NULL, NULL );
	*/
}

char* ClientConnect::GetSendBuffAddr()
{
	return mSendBuffer;
}

void ClientConnect::SendPacket(unsigned long packetSize)
{
	int ret = 0;
	packet_header *header = reinterpret_cast<packet_header*>(mSendBuffer);
	header->size = packetSize;
	mSendWSAbuf.len = packetSize;

	DWORD iobyte;
	//send( mSocket, mSendBuffer, header->size, NULL );
	ret = WSASend(mSocket, &mSendWSAbuf, 1, &iobyte, 0, NULL, NULL);
	if (ret)
	{
		int errorCode = WSAGetLastError();
		std::cout << "Error Code [" << errorCode << "] \n";
	}
}

bool ClientConnect::ThereIsProcessPacket()
{
	/*CONNECT.mDataQueueLock.ReadLock();
	bool result = !mDataQueue.empty();
	CONNECT.mDataQueueLock.ReadUnLock();
	return result;*/
	return false;
}

PacketStore ClientConnect::GetPacket()
{
	/*CONNECT.mDataQueueLock.WriteLock();
	PacketStore packet= mDataQueue.back();
	mDataQueue.pop();
	CONNECT.mDataQueueLock.WriteUnLock();
	return packet;*/
	return PacketStore();
}