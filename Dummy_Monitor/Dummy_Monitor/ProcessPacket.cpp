#include "stdafx.h"

//void ProcessMovePacket(int user_id, char *packet)
//{
//	struct sc_packet_pos *movePacket = (struct sc_packet_pos *)packet;
//
////	if (movePacket->id >= TEST_PLAYER_NUM) return;
//	if (user_id != movePacket->id) return;
//
//	player[user_id].x = movePacket->x;
//	player[user_id].y = movePacket->y;
//}
//
//
//void ProcessPutObjectPacket(int user_id, char *packet)
//{
//	struct sc_packet_put_player *PutObject = reinterpret_cast<sc_packet_put_player *>(packet);
//
//	if (PutObject->id >= TEST_PLAYER_NUM) return;
//	if (false == player[user_id].id_received) {
//		player[user_id].obj_id = PutObject->id;
//	}
//
//	player[user_id].x = PutObject->x;
//	player[user_id].y = PutObject->y;	
//}

bool ProcessPacket(int user_id, char *packet)
{
	switch (packet[1]) {
	case SC_LOGIN_OK:
	{
		sc_packet_login_ok *okPacket = reinterpret_cast<sc_packet_login_ok*>(packet);
		if (okPacket->id >= TEST_PLAYER_NUM) return true;
		player[okPacket->id].x = okPacket->x_pos;
		player[okPacket->id].y = okPacket->y_pos;

		break;
	}
	case SC_ADD_OBJECT:
	{
		sc_packet_add_object *addPacket = reinterpret_cast<sc_packet_add_object*>(packet);
		addPacket->id;
		if (addPacket->id >= TEST_PLAYER_NUM) return true;
		if (false == player[user_id].id_received) {
			player[user_id].obj_id = addPacket->id;
		}

		player[user_id].x = addPacket->x_pos;
		player[user_id].y = addPacket->y_pos;
		if (user_id == 1)
			std::cout << player[user_id].x << ", " << player[user_id].y << std::endl;
		break;
	}
	case SC_POSITION_INFO:
	{
		sc_packet_position_info *PutObject = reinterpret_cast<sc_packet_position_info *>(packet);

		player[user_id].x = PutObject->x_pos;
		player[user_id].y = PutObject->y_pos;
		break;
	}
	

		/*case SC_PUT_PLAYER:
			ProcessPutObjectPacket(user_id, packet);
			break;

		case SC_POS:
			ProcessMovePacket(user_id, packet);
			break;*/

		//case SC_REMOVE_PLAYER:
		////	ProcessRemovePacket(packet);
		//	break;
	default:

		break;
	}
	

	return TRUE;
}