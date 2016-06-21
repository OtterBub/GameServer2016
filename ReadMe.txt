●Folder 설명 
 
----------- ClientFrameWork --------------- 
GameClient의 프레임이 되는 프로젝트 폴더 입니다. 
해당 프레임은 
https://github.com/OtterBub/2014-2-ComputerGraphics_TA/tree/master/TA 
의 프로젝트와 같습니다. 
 
 
----------- DataBaseBackUp --------------- 
DB 백업이 있는 폴더입니다. 
 
 
Admin ID : sa 
Password : gamedb 
 
 
obdc name : 2010180020 
DB name : 2010180020 
 
 
계정 입력 부분은 
GameServer\Source\System\MGR\DataBaseMgr.cpp 의 
75줄에 있습니다. 
 
 
----------- Dummy_Monitor --------------- 
교수님께서 주신 더미 모니터 기반으로 
제 확장 패킷을 적용하여 더미 클라이언트를 만들었습니다. 
 
 
----------- GameClient --------------- 
GameClient의 소스가 들어있는 폴더입니다. 
 
 
----------- GameServer --------------- 
GameServer의 소스가 들어있는 폴더입니다. 
 
 
----------- lib --------------- 
클라이언트들의 공통 lib가 들어있는 폴더입니다. 
 
 
----------- Output --------------- 
GameServer.sln으로 빌드하는 모든 결과물은 
이곳으로 나옵니다. 
 
 
D_projectname 은 디버그 빌드 결과물이며 
projectname은 릴리즈 모드 결과물입니다. 
 
 
디버그 결과물은 빼고 제출 합니다. 


●Output 폴더에 실행 파일이 있습니다.
---------- GameClient -------------
기본적으로 Scene Change가 있을때마다 커스텀 콘솔이 뜹니다.
콘솔 입력창에 ip를 입력하면 MainMenu가 뜨게됩니다.

1. MMO Play 버튼 클릭

2. ' ` ' (물결 특수기호) 키를 눌르기

3. /login game_id를 입력하여 게임 로그인

4. ' ` ' (물결 특수기호) 키를 다시 눌러 콘솔창 지우기

5. 쉬프트 + ' ` ' (물결 특수기호) 키를 누르면 보기모드로 콘솔창이 띄워집니다.

6. 스페이스바를 누르면 공격 메세지가 보입니다.

7. /logout을 입력하면 logout 패킷 전송과 동시에 메인메뉴로

8. 강제 종료시에도 디비저장 가능!

---------------- 조작법 -------------

숫자키 '1' 왼쪽에 있는 ' ` ' (물결 특수기호) 키
- 입력모드로 콘솔창 보이기

쉬프트 + ' ` ' (물결 특수기호) 키
- 보기모드로 콘솔창 보이기

w a s d : 캐릭터 방향 조작
SpaceBar : 캐릭터 공격

-------------- 콘솔 명령어 ------------------
/login your_id
- your_id라는 ID로 접속

/logout
- logout 패킷을 보내고 게임메뉴로

/exit
- 프로그램 종료 (강제 종료시에도 db저장 가능)

-------- 기타 명령어 --------------
/depth on / off
- 깊이 테스트 on / off

/3d on / off
- 3d 모드 켜고 끄기 (MMO Scene에서는 제대로 된 동작 안됨)

---------- 기타 Scene -----------
해당 github 프로젝트 폴더에 있는 프레임을 기반으로
MMO 프로젝트 씬만 추가하여 이번 텀프로젝트를 진행해 보았습니다.

https://github.com/OtterBub/2014-2-ComputerGraphics_TA/tree/master/TA/Output
 
  
●GameClient Class 설명 
주 클래스만 설명 하겠습니다. 
 
 
■System  
------------- ThreadMgr ------------- 
std::thread를 관리하는 매니저 클래스 입니다. 
 
 
------------- ClientConnect ------------- 
클라이언트의 서버 접속을 담당하는 클래스입니다. 
ReadPacket 함수와 SendPacket 함수가 만들어져있습니다. 
 
 
■ClientFrame\System 
------------- RWLock ------------- 
ReadLock WriteLock을 구현하기 위한 클래스입니다. 
 
 
■ClientFrame\System\Scene 
------------- SceneMMO ------------- 
MMO Play 씬이 구현되어있는 클래스입니다. 
 
 
 
●GameServer Class 설명 
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
 
 
 