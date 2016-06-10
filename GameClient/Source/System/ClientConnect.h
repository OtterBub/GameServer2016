#ifndef  __CLIENT_CONNECT_H__
#define __CLIENT_CONNECT_H__

#include "../Common.h"

#define BUF_SIZE 1024
#define SERVERPORT 

#define CONNECT ClientConnect::GetInstance()

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
};

#endif
