#pragma once

#include <sys/sem.h>

class SharedMemControl
{
public:
	explicit SharedMemControl();
	virtual ~SharedMemControl();

	void StartSharedMemProcess();

private:
	char* pWriteShmAddr;
};