#pragma once

#include <sys/sem.h>
#include <atomic>

#include "ISharedMemControl.h"

class SharedMemControl : public ISharedMemControl
{
public:
	explicit SharedMemControl();
	virtual ~SharedMemControl();

	void StartSharedMemProcess();

	static void* RecvThreadEntry(void* argument);
	virtual void RecvSharedMemMessage() override;
	virtual bool SendSharedMemMessage(const std::string& message) override;

	void Wait();
	void Stop();	

private:
	//SharedMem Write
	char* pWriteShmAddr = nullptr;
	int nSimToNetShmSize = 0;
	int sendSemId = -1;

	// SharedMem read
	int nNetToSimShmSize = 0;
	char* pReadShmAddr = nullptr;
	int recvSemId = -1;
	std::atomic<bool> recvRunning{ false };
	bool recvThreadStarted = false;

	pthread_t recvThread{};
};