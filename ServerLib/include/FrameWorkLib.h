#pragma once

#include <stdio.h>
#include <tchar.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "Winmm.lib")


#include <WS2tcpip.h>
#include <WinSock2.h>
#include <mswsock.h>
#include <mmsystem.h>
#include <WS2spi.h>
#include <mstcpip.h>
#include <process.h>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <io.h>
#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <assert.h>
#include <fcntl.h>
#include <algorithm>
#include <functional>

#include <thread>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <memory>

#include <string>
#include <tchar.h>

#include <ctime>
#include <random>
#include <typeinfo>

#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
#include <list>
#include <array>
#include <atomic>
#include <deque>
#include <queue>
#include <concurrent_queue.h>
#include <condition_variable>

#if _DEBUG
#define	CONTEXT_SWITCH		Sleep(1)
#else
#define	CONTEXT_SWITCH		::SwitchToThread()
#endif


#ifndef   _DEBUG
#pragma comment(lib, "libtcmalloc_minimal")
#pragma comment(linker, "/include:__tcmalloc")
#endif


#include "Singleton.h"
#include "Type.h"
#include "Defines.h"
#include "AsyncLogMsg.h"
#include "Log.h"
#include "Packet.h"
#include "Utility.h"
#include "Job.h"
#include "WinsockHelper.h"
#include "IPv4.h"

#include "Iocp.h"
#include "Overlapped.h"
#include "Handler.h"
#include "Dispatcher.h"

#include "EXFunction.h"
#include "Listener.h"
#include "ThreadLocal.h"

#include "Lock.h"

#include "Thread.h"
#include "NetworkIoThread.h"
#include "ThreadDepartment.h"
#include "ThreadManager.h"
#include "SendBufferQueue.h"

#include "IoProcessor.h"
#include "AcceptProcessor.h"
#include "SendProcessor.h"
#include "ReceiveProcessor.h"

#include "NetworkDepartment.h"
#include "ClientNetWorkDepartment.h"
#include "ServerNetWorkDepartment.h"
#include "NetworkDepartmentManager.h"

#include "Session.h"
#include "SessionPool.h"
#include "Server.h"
