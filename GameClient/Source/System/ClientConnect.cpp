#include "ClientConnect.h"

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
	packet_header *header = reinterpret_cast<packet_header*>(mSendBuffer);
	header->size = packetSize;
	mSendWSAbuf.len = packetSize;

	DWORD iobyte;
	int ret = WSASend(mSocket, &mSendWSAbuf, 1, &iobyte, 0, NULL, NULL);
	if (ret)
	{
		int errorCode = WSAGetLastError();
		std::cout << "Error Code [" << errorCode << "] \n";
	}
}