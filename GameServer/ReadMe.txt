GameServer Class ����
�� Ŭ������ ���� �ϰڽ��ϴ�.


-------------- PacketStruct --------------

packet_header
BYTE size
BYTE type
���� ��Ŷ �������� ���� ��� ����


- CLIENT TO SERVER

cs_packet_login
�α��νÿ� Ŭ���̾�Ʈ���� ������ ��Ŷ

cs_packet_move
Ű���带 ������ �ÿ� �������� ��Ŷ

enum moveDir
MOVE_RIGHT = 0x0001
MOVE_LEFT = 0x0002
MOVE_UP = 0x0004
MOVE_DOWN = 0x0008

��� ���� �ξ�
8������ ������ �� �ְ� �߽��ϴ�.

cs_packet_attack
�����ߴٴ� ��Ŷ

- SERVER TO CLIENT

sc_packet_login_ok
�α����� �Ǿ��ٴ� ��Ŷ���� DB�� ����Ǿ��ִ� ������ �������ش�.

sc_packet_position_info
� Ÿ���� ��ü�� ��ġ ������ �ִ� ��Ŷ

sc_packet_add_object
Ŭ���̾�Ʈ ������Ʈ �߰��� ���� ��Ŷ

sc_packet_remove_object
Ŭ���̾�Ʈ ������Ʈ ���Ÿ� ���� ��Ŷ

sc_packet_stat_change
level, hp, exp�� ������ �ٲ�� ���۵Ǵ� ��Ŷ

sc_packet_attack_object
���� �޼����� �����ϸ� ���� ������ �����ߴ����� ����
������ ����ִ� ��Ŷ

��System
-------------- Connect --------------
�̹� �б��� ���Ӽ��� �ð��� ��� ��� ���İ� �ڵ尡 ���ִ� Ŭ����
AcceptThread, WorkerThread�� ����
SendPacket, ProcessPacket�� ��� ó���Ѵ�.


��System\MGR
-------------- DataBaseMgr --------------
DB������ ���� ����, ���� ���� ������ �Ǵ� DB���� �Ŵ��� Ŭ����

-------------- EventMgr --------------
�̺�Ʈ ������ ����Ͽ� iocp workerThread���� �۾��ϰԲ� ���ִ� �̺�Ʈ �Ŵ��� Ŭ����

-------------- MTObjectMGR --------------
��ü ���� �޴��� Ŭ����,
��Ƽ �����忡�� ������ �����ϰԲ� RWLock�� �����Ͽ�����
�⺻������ �̱��� ��ü�� ����,
std::map�� ���Ͽ� � ��ġ������ ��ü�� ���� �ҷ��� �� �ִ�.

�������� ���带 �����ϱ� ���Ͽ� �̱��� ��ü�� map �ε����� ����
�ҷ����� ��������� ���� ���� �̱��� ��Ȳ

-------------- ObjectMgrList --------------
MTObjectMGR Ŭ������ �̿��� ��ü�� �����ϱ� ���� ���
�ش� ������Ʈ������ 
CLIENT �� NPC�� �����մϴ�.

-------------- ThreadMgr --------------
std::thread Ŭ������ �����ϱ� ���� �Ŵ��� Ŭ����

