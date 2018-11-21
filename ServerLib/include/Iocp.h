#pragma once

class Iocp
{
private:
	
	HANDLE		iocp_;
	Bool		isRunning_;

public:

	Iocp(DWORD numberOfConcurrentThreads = 0);
	~Iocp();

	Void			Resister(HANDLE handle, ULONG_PTR completionkey = 0);
	
	Void			ListenToCompletionPort();
	Void			ClosePort();
	Void			Stop();
	
	HANDLE			GetHandle() const;


};