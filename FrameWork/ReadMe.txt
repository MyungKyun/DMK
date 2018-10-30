2018-10-23

일부 작성시키고 테스트한 라이브러리 코드를 처음 커밋했다.
서버-클라 사이에 테스트용 패킷은 MessagePacket을 주고 받는 것을 테스트 하였다.
잘 동작한다. 패킷처리시 아직 동시성 문제는 해결하지 않았다. 잘 알려진 Job처리를 넣을 예정
추후에 구글프로토버퍼를 붙일 예정이다. 

ServerNetWorkDepartment 가 서버를 작동하는 클래스이다.

	- ***Processor 라고 이름 붙여진 멤버들은 GQCS의 Network-IO 이벤트를 처리 하기 위해 존재한다.
	
	- ***Processor 중 Send, Recv는 Session 클래스에서 Pointer로 가지고 있다.
	  Send, Recv 는 Session이 처리하도록 고민해봄직 하다. 너무 타고들어가는 느낌이 있다.

ThreadManager
	
	-  생성된 Thread들을 Department 라는 enum을 두어 저장해두고 있다.
	-  ThreadId는 TLS 로 저장해두고 있다.
	-  Server 클래스와 , PacketProcess 클래스 에서 따로 Thread Array를 멤버로 가지고 있는데 필요가 없을 듯하다. 
               (추후 삭제하자)

2018-10-28
	
	- SendBufferQueue로 Concurrent_Queue를 사용해보기 위해 새로 추가하였다.
            - 아직 정확하게 동작할 코드는 작성하지 않았다. 
	    
	SendBufferQueue 로 Send가 정상 동작한다면 그 후에 구글 프로토버퍼를 붙여볼 계획이다.

2018-10-29
	
	- ThreadDepartment 클래스를 추가 하였다. 해당 클래스는 생성된 Thread들을 unique_ptr로 관리하며
	  ThreadManager 클래스에서 Thread 생성하기전 Department 를 먼저 생성하고, Thread를 생성하여야 동작한다.
              이렇게 나눈 이유는 추후 Thread에 Job을 넘길때 어떤 ThreadDepartment 에 넘길지 선택하기 위함이다.

2018-10-30

	- Session 클래스의 sessionId를 발급하기 위한 IdGenerator 싱글톤 클래스를 추가하였다.
	  Session이 생성될때 sessionId가 발급되고, 해당 ID 는 서버 프로그램이 종료될때까지 바뀌지 않는다.
              이전에는 SessionPool에서 Session을 가져올때 ID를 발급해주었는데 이렇게 되면 Lock을 걸어야해서
              최초 서버 구동시 아이디를 모두 발급해버리게 수정 하였다.

	- Lock 클래스의 LockSafeScope, LockManager 클래스를 삭제하였다. 단순히 recursive_mutex로 Lock을 걸었던걸 변경하여
              shared_mutex, shared_lock, unique_lock 으로 사용하도록 한다. 매크로 추가.

	% Google protobuffer 대신 flatbuffer를 사용해 보는걸 고려해봄직하다. 자료를 찾아보니 protobuf와 다르게 직렬화시 모델에 값을 채우는게 바이트어레이고
	 역직렬화시 바이트어레이가 곧 모델이라 복사하는 과정이 없어 성능이 좋다고한다.  사용시에 코드 작성량이 조금 많다는게 단점이라고 한다.
                          
              이외에도 ZeroFormatter 라는 직/역직렬화 라이브러리가 있는데 C#만 지원하는것 같다. C#에선 압도적인 성능을 보이는것같다. 
