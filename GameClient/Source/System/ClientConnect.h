#ifndef  __CLIENT_CONNECT_H__
#define __CLIENT_CONNECT_H__

#include "../Common.h"
#include "../ClientFrame/System/RWLock.h"

#define BUF_SIZE 1024
#define SERVERPORT 

#define CONNECT ClientConnect::GetInstance()

struct PacketStore
{
	char packet[BUF_SIZE];
};

class ClientConnect
{
public:
	static ClientConnect& GetInstance()
	{
		static ClientConnect inst;
		return inst;
	}

	static void ReadPacket();
	static void ProcessPacket( char *packet );

	void Initialize();
	void Connect( std::string ipAddr );

	char* GetSendBuffAddr();

	template <class T>
	T* GetSendBuffAddr()
	{
		return reinterpret_cast<T*>(mSendBuffer);
	}

	void SendPacket( unsigned long packetSize );

	bool ThereIsProcessPacket();
	PacketStore GetPacket();

	RWLock mConnectLock;
	int mMyID;

private:
	ClientConnect() {};
	ClientConnect(const ClientConnect& tmp) {};

private:
	SOCKET mSocket;

	WSABUF mSendWSAbuf;
	char mSendBuffer[BUF_SIZE];

	WSABUF mRecvWSAbuf;
	char mRecvBuffer[BUF_SIZE];

	char mPacketBuffer[BUF_SIZE];

	DWORD mInPacketSize;
	int mSavedPacketSize;

	bool mShutDown;

	//std::queue<PacketStore> mDataQueue;
};

#endif
