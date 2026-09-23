#include <unistd.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <limits.h>
#include <filesystem>
#include <pthread.h>
#include <algorithm>
#include <cstdint>
#include <cstddef>
#include <cstring>

#include "SharedMemControl.h"
#include "CommonUtil.h"

SharedMemControl::SharedMemControl()
	: pReadShmAddr(nullptr),
	pWriteShmAddr(nullptr)	
{
}

SharedMemControl::~SharedMemControl()
{
}

void SharedMemControl::StartSharedMemProcess()
{
	char szExePath[PATH_MAX];
	ssize_t nLength = readlink("/proc/self/exe", szExePath, sizeof(szExePath) - 1);
	if (nLength == -1)
	{
		return;
	}

	szExePath[nLength] = '\0';
	std::filesystem::path ExePath(szExePath);
	std::filesystem::path ExeDir = ExePath.parent_path();
	std::filesystem::path ConfigPath = ExeDir / ".." / "Resource" / "ipc.cfg";
	std::string strConfigPath = std::filesystem::weakly_canonical(ConfigPath).string();

	std::string strSectionName = "SHARED_MEMORY_INFO";

	// Engine Read
	std::string strSimToNetKey = "NET_TO_SIM_SHM_KEY";
	const int nSimToNetKey = std::stoi(CCommonUtility::GetValue(strConfigPath, strSectionName, strSimToNetKey));

	std::string strSimToNetSize = "NET_TO_SIM_SHM_SIZE";
	nNetToSimShmSize = std::stoi(CCommonUtility::GetValue(strConfigPath, strSectionName, strSimToNetSize));

	// SharedMemory Create
	const int nSimToNetId = shmget(static_cast<key_t>(nSimToNetKey), nNetToSimShmSize, IPC_CREAT | 0666U);
	if (nSimToNetId < 0)
	{
		std::cout << "[ERROR] " << std::endl;
	}

	void* pAddr = shmat(nSimToNetId, nullptr, 0);
	if (pAddr == reinterpret_cast<void*>(-1))
	{
		perror("shmat");
		return;
	}
	pReadShmAddr = static_cast<char*>(pAddr);	

	const int nNetToSimSemKey = std::stoi(CCommonUtility::GetValue(strConfigPath, strSectionName, "NET_TO_SIM_SEM_KEY"));
	
	// 시뮬레이션 서버가 세마포어 집합을 최초 생성
	recvSemId = semget(static_cast<key_t>(nNetToSimSemKey),	2,IPC_CREAT | IPC_EXCL | 0660);
	if (recvSemId >= 0)
	{
		unsigned short initialValues[2] = { 1, 0 };  // EMPTY=1, FULL=0
		
		union Semun
		{
			int val;
			struct semid_ds* buf;
			unsigned short* array;
		};

		Semun arg{};
		arg.array = initialValues;

		if (semctl(recvSemId, 0, SETALL, arg) == -1)
		{
			perror("semctl SETALL");
			return;
		}
	}
	else if (errno == EEXIST)
	{
		// 이미 생성되어 있으면 다시 초기화하지 않음
		recvSemId = semget(static_cast<key_t>(nNetToSimSemKey), 2, 0660);
	}

	if (recvSemId == -1)
	{
		perror("semget NET_TO_SIM");
		return;
	}

	pReadShmAddr = static_cast<char*>(pAddr);

	recvRunning.store(true);
	const int result = pthread_create(&recvThread, nullptr, &SharedMemControl::RecvThreadEntry, this);

	if (result != 0)
	{
		std::cerr << "pthread_create failed: " << result << '\n';
		recvRunning.store(false);
		return;
	}

	recvThreadStarted = true;
}

void* SharedMemControl::RecvThreadEntry(void* argument)
{
	auto* self = static_cast<SharedMemControl*>(argument);
	self->RecvSharedMemMessage();
}

void SharedMemControl::RecvSharedMemMessage()
{
	constexpr std::size_t LENGTH_SIZE = sizeof(std::uint32_t);
	constexpr std::size_t MESSAGE_CAPACITY = 4096;
	const std::size_t maxLength = std::min(MESSAGE_CAPACITY, static_cast<std::size_t>(nNetToSimShmSize) - LENGTH_SIZE);

	while (recvRunning.load())
	{
		// UI 서버가 메시지 기록을 마치고 FULL을 +1 할 때까지 대기
		sembuf waitFull{};
		waitFull.sem_num = 1;  // FULL
		waitFull.sem_op = -1;

		timespec timeout{};
		timeout.tv_sec = 1;

		if (semtimedop(recvSemId, &waitFull, 1, &timeout) == -1)
		{
			if (errno == EAGAIN || errno == EINTR)
			{
				continue;
			}

			perror("semtimedop FULL");
			break;
		}

		// pReadShmAddr[0..3]: 메시지 길이
		std::uint32_t length = 0;
		std::memcpy(&length, pReadShmAddr, LENGTH_SIZE);

		std::string message;
		const bool validLength = length <= maxLength;

		if (validLength)
		{
			// pReadShmAddr[4..]: 메시지 내용
			message.assign(pReadShmAddr + LENGTH_SIZE, length);
		}

		// 복사가 끝났으므로 UI 서버가 다음 메시지를 쓸 수 있게 함
		sembuf postEmpty{};
		postEmpty.sem_num = 0;  // EMPTY
		postEmpty.sem_op = +1;

		if (semop(recvSemId, &postEmpty, 1) == -1)
		{
			perror("semop EMPTY");
			break;
		}

		if (!validLength)
		{
			std::cerr << "Invalid message length: " << length << '\n';
			continue;
		}

		std::cout << "[Simulation received] " << message << '\n';
	}
}