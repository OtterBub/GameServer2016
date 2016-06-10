#include <string>
#include <thread>

#include "../GameClient/Source/System/ClientConnect.h"

int main()
{
	CONNECT.Initialize();

	CONNECT.Connect( "127.0.0.1" );

	std::thread *readPacketThread = new std::thread{ CONNECT.ReadPacket };

	while (1)
	{
		static int move = 0;;

		//cs_packet_move *packet = reinterpret_cast<cs_packet_move*>(CONNECT.GetSendBuffAddr());
		cs_packet_move *packet = CONNECT.GetSendBuffAddr<cs_packet_move>();
		
		packet->header.type = 1;
		packet->moveDir = ++move;

		CONNECT.SendPacket( sizeof(cs_packet_move) );
		Sleep(1000);
	}

	readPacketThread->join();
	delete readPacketThread;

	return 0;
}