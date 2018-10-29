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
