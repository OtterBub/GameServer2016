#include "ClientConnect.h"

void ClientConnect::Initialize()
{
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
}

void ClientConnect::Connect(std::string ipAddr)
{

}