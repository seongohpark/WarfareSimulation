#pragma once

#include <sys/sem.h>
#include <atomic>

class SharedMemControl
{
public:
	explicit SharedMemControl();
	virtual ~SharedMemControl();

	void StartSharedMemProcess();

	static void* RecvThreadEntry(void* argument);
	void RecvSharedMemMessage();

private:
	char* pWriteShmAddr;

	int nNetToSimShmSize = 0;
	char* pReadShmAddr;
	int recvSemId = -1;
	std::atomic<bool> recvRunning{ false };
	bool recvThreadStarted = false;

	pthread_t recvThread{};
};