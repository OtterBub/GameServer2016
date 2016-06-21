GameServer Class 설명
주 클래스만 설명 하겠습니다.


-------------- PacketStruct --------------

packet_header
BYTE size
BYTE type
으로 패킷 공통으로 들어가는 헤더 선언


- CLIENT TO SERVER

cs_packet_login
로그인시에 클라이언트에서 보내는 패킷

cs_packet_move
키보드를 눌렀을 시에 보내지는 패킷

enum moveDir
MOVE_RIGHT = 0x0001
MOVE_LEFT = 0x0002
MOVE_UP = 0x0004
MOVE_DOWN = 0x0008

라는 값을 두어
8방향을 움직일 수 있게 했습니다.

cs_packet_attack
공격했다는 패킷

- SERVER TO CLIENT

sc_packet_login_ok
로그인이 되었다는 패킷으로 DB에 저장되어있던 정보를 전송해준다.

sc_packet_position_info
어떤 타입의 객체의 위치 정보를 주는 패킷

sc_packet_add_object
클라이언트 오브젝트 추가를 위한 패킷

sc_packet_remove_object
클라이언트 오브젝트 제거를 위한 패킷

sc_packet_stat_change
level, hp, exp의 스텟이 바뀌면 전송되는 패킷

sc_packet_attack_object
공격 메세지에 응답하며 누가 누구를 공격했는지에 대한
정보가 담겨있는 패킷

■System
-------------- Connect --------------
이번 학기의 게임서버 시간에 배운 모든 지식과 코드가 들어가있는 클래스
AcceptThread, WorkerThread는 물론
SendPacket, ProcessPacket을 모두 처리한다.


■System\MGR
-------------- DataBaseMgr --------------
DB서버에 대한 정보, 쿼리 등을 얻어오게 되는 DB서버 매니저 클래스

-------------- EventMgr --------------
이벤트 정보를 등록하여 iocp workerThread에서 작업하게끔 해주는 이벤트 매니저 클래스

-------------- MTObjectMGR --------------
객체 관리 메니저 클래스,
멀티 쓰레드에서 동작이 가능하게끔 RWLock을 적용하였으며
기본적으로 싱글톤 객체로 생성,
std::map을 통하여 어떤 위치에서든 객체를 쉽게 불러올 수 있다.

여러개의 월드를 구현하기 위하여 싱글톤 객체를 map 인덱스를 통해
불러오게 만들었으나 여러 월드 미구현 상황

-------------- ObjectMgrList --------------
MTObjectMGR 클래스를 이용한 객체를 관리하기 위한 헤더
해당 프로젝트에서는 
CLIENT 와 NPC를 관리합니다.

-------------- ThreadMgr --------------
std::thread 클래스를 관리하기 위한 매니저 클래스

