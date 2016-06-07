#include "ClientConnect.h"

void ClientConnect::ReadPacket()
{
	DWORD iobyte = 0, ioflag = 0;
	while (false == CONNECT.mShutDown)
	{
		//int ret = WSARecv(CONNECT.mSocket, &CONNECT.mRecvWSAbuf, 1, &iobyte, &ioflag, NULL, NULL);
		//if (ret)
		//{
		//	int err_code = WSAGetLastError();
		//	std::cout << "Recv Error [" << err_code << "]\n";
		//}

		//BYTE *ptr = reinterpret_cast<BYTE*>(CONNECT.mRecvBuffer);

		//while (0 != iobyte)
		//{
		//	if (0 == CONNECT.mInPacketSize) CONNECT.mInPacketSize = ptr[0];
		//	if (iobyte + CONNECT.mSavedPacketSize >= CONNECT.mInPacketSize)
		//	{
		//		memcpy(CONNECT.mPacketBuffer + CONNECT.mSavedPacketSize, ptr, CONNECT.mInPacketSize - CONNECT.mSavedPacketSize);
		//		// ProcessPacket
		//		ClientConnect::ProcessPacket(CONNECT.mPacketBuffer);

		//		ptr += CONNECT.mInPacketSize - CONNECT.mSavedPacketSize;
		//		iobyte -= CONNECT.mInPacketSize - CONNECT.mSavedPacketSize;
		//		CONNECT.mInPacketSize = 0;
		//		CONNECT.mSavedPacketSize = 0;
		//	}
		//	else
		//	{
		//		memcpy(CONNECT.mPacketBuffer + CONNECT.mSavedPacketSize, ptr, iobyte);
		//		CONNECT.mSavedPacketSize += iobyte;
		//		iobyte = 0;
		//	}
		//}

		int received;
		char *ptr = CONNECT.mRecvBuffer;
		int left = -1;

		while ((-1 == left) || (left > 0))
		{
			if (-1 == left)
			{
				received = recv(CONNECT.mSocket, ptr, sizeof(packet_header::size), 0);
				left = ptr[0];
				ptr += received;
			}
			else
			{
				received = recv(CONNECT.mSocket, ptr, left, 0);
				if (0 == received)
					break;
				left -= received;
				ptr += received;
			}
		}

		ClientConnect::ProcessPacket(CONNECT.mPacketBuffer);
	}
}

void ClientConnect::ProcessPacket(char *packet)
{
	packet_header *header = reinterpret_cast<packet_header*> (packet);
	std::cout << "[Recv] type: " << (int)header->type << " size: " << (int)header->size << std::endl;
	switch (header->type)
	{
	case SC_TYPE_MOVE:
	{
		sc_packet_player_pos *posPacket = reinterpret_cast<sc_packet_player_pos*>(packet);
		std::cout << "SC_TYPE_MOVE (" <<
			posPacket->x << ", " <<
			posPacket->y << ") \n";


		break;
	}
	default:
		std::cout << "UnKnown Packet Type: " << (int)header->type << std::endl;
		break;
	}
}

void ClientConnect::Initialize()
{
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

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
	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	SOCKADDR_IN lServerAddr;
	memset(&lServerAddr, 0, sizeof(lServerAddr));
	lServerAddr.sin_family = AF_INET;
	lServerAddr.sin_port = htons(SERVER_PORT);
	lServerAddr.sin_addr.s_addr = inet_addr( ipAddr.c_str() );

	int Result = WSAConnect( mSocket, (sockaddr*)&lServerAddr, sizeof(lServerAddr), NULL, NULL, NULL, NULL );
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

	for (int i = 0; i < 10; ++i)
	{
		std::cout << i;
	}
	std::cout << "\n";
}