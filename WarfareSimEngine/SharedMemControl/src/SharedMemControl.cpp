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
{
}

SharedMemControl::~SharedMemControl()
{
	recvRunning.store(false);

	if (recvThreadStarted)
	{
		pthread_join(recvThread, nullptr);
		recvThreadStarted = false;
	}

	if (pReadShmAddr != nullptr)
	{
		shmdt(pReadShmAddr);
		pReadShmAddr = nullptr;
	}

	if (pWriteShmAddr != nullptr)
	{
		shmdt(pWriteShmAddr);
		pWriteShmAddr = nullptr;
	}
}

void SharedMemControl::StartSharedMemProcess()
{
	if (recvThreadStarted)
	{
		std::cerr << "[WARNING] Shared memory process already started\n";
		return;
	}

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

	// -------------------------------------------------------
	// Engine Write : Simulation -> Network/UI
	// -------------------------------------------------------
	const std::string strSimToNetKey = "SIM_TO_NET_SHM_KEY";
	const int nSimToNetKey = std::stoi(CCommonUtility::GetValue(strConfigPath, strSectionName, strSimToNetKey));

	const std::string strSimToNetSize = "SIM_TO_NET_SHM_SIZE";
	nSimToNetShmSize =std::stoi(CCommonUtility::GetValue(strConfigPath,	strSectionName, strSimToNetSize));

	// Write용 Shared Memory 생성 또는 기존 Shared Memory 획득
	const int nSimToNetId =	shmget(static_cast<key_t>(nSimToNetKey), nSimToNetShmSize, IPC_CREAT | 0666U);	

	if (nSimToNetId < 0)
	{
		perror("shmget SIM_TO_NET");
		return;
	}

	// Write용 Shared Memory Attach
	void* pWriteAddr = shmat(nSimToNetId, nullptr, 0);

	if (pWriteAddr == reinterpret_cast<void*>(-1))
	{
		perror("shmat SIM_TO_NET");
		return;
	}

	// 핵심: Write Shared Memory 주소 저장
	pWriteShmAddr =	static_cast<char*>(pWriteAddr);
	
	// Write용 Semaphore 생성
	const int nSimToNetSemKey =	std::stoi(CCommonUtility::GetValue(strConfigPath, strSectionName, "SIM_TO_NET_SEM_KEY"));

	sendSemId =	semget(static_cast<key_t>(nSimToNetSemKey),	2, IPC_CREAT | IPC_EXCL | 0660);
	if (sendSemId >= 0)
	{
		// sem[0] = EMPTY
		// sem[1] = FULL
		unsigned short initialValues[2] = {	1, 0 };  // EMPTY=1, FULL=0

		union Semun
		{
			int val;
			struct semid_ds* buf;
			unsigned short* array;
		};

		Semun arg{};
		arg.array = initialValues;

		if (semctl(sendSemId, 0, SETALL, arg) == -1)
		{
			perror("semctl SIM_TO_NET SETALL");
			return;
		}
	}
	else if (errno == EEXIST)
	{
		// 이미 생성되어 있으면 기존 세마포어 사용
		sendSemId =	semget(static_cast<key_t>(nSimToNetSemKey),	2, 0660);
	}

	if (sendSemId == -1)
	{
		perror("semget SIM_TO_NET");
		return;
	}

	std::cout << "[Write] : KEY - " << nSimToNetKey << ", SIZE - " << nSimToNetShmSize << ", ID - " << nSimToNetId << ", SEM KEY - " << nSimToNetSemKey << std::endl;

	// -------------------------------------------------------
	// Engine Read : Network/UI -> Simulation
	// -------------------------------------------------------
	std::string strNetToSimKey = "NET_TO_SIM_SHM_KEY";
	const int nNetToSimKey = std::stoi(CCommonUtility::GetValue(strConfigPath, strSectionName, strNetToSimKey));

	std::string strNetToSimSize = "NET_TO_SIM_SHM_SIZE";
	nNetToSimShmSize = std::stoi(CCommonUtility::GetValue(strConfigPath, strSectionName, strNetToSimSize));

	// SharedMemory Create
	const int nNetToSimId = shmget(static_cast<key_t>(nNetToSimKey), nNetToSimShmSize, IPC_CREAT | 0666U);
	if (nNetToSimId < 0)
	{
		perror("shmget NET_TO_SIM");
		return;
	}

	void* pAddr = shmat(nNetToSimId, nullptr, 0);
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

	std::cout << "[Read] : KEY - " << nNetToSimKey << ", SIZE - " << nNetToSimShmSize << ", ID - " << nNetToSimId << ", SEM KEY - " << nNetToSimSemKey << std::endl;

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

	return nullptr;
}

void SharedMemControl::RecvSharedMemMessage()
{
	std::cout << "[RecvThread] Start" << std::endl;

	if (recvSemId == -1)
	{
		std::cerr << "[ERROR] recvSemId is invalid\n";
		return;
	}

	constexpr std::size_t LENGTH_SIZE = sizeof(std::uint32_t);
	if (nNetToSimShmSize <= static_cast<int>(LENGTH_SIZE))
	{
		std::cerr << "[ERROR] Invalid NET_TO_SIM shared memory size\n";
		return;
	}
	
	const std::size_t maxLength = static_cast<std::size_t>(nNetToSimShmSize) - LENGTH_SIZE;

	while (recvRunning.load())
	{
		//std::cout << "[RecvThread] Waiting FULL..." << std::endl;
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

bool SharedMemControl::SendSharedMemMessage(const std::string& message)
{
	constexpr std::size_t LENGTH_SIZE = sizeof(std::uint32_t);

	if (pWriteShmAddr == nullptr)
	{
		std::cerr << "[ERROR] pWriteShmAddr is null\n";
		return false;
	}

	// 세마포어가 정상적으로 생성되었는지 확인
	if (sendSemId == -1)
	{
		std::cerr << "[ERROR] sendSemId is invalid\n";
		return false;
	}

	if (nSimToNetShmSize <= static_cast<int>(LENGTH_SIZE))
	{
		std::cerr << "[ERROR] Invalid SIM_TO_NET shared memory size\n";
		return false;
	}

	const std::size_t maxLength = static_cast<std::size_t>(nSimToNetShmSize) - LENGTH_SIZE;
	
	if (message.size() > maxLength)
	{
		std::cerr << "[ERROR] Message too large. size="	<< message.size() << ", max=" << maxLength << '\n';
		return false;
	}

	// ----------------------------------------------------
	// 1. EMPTY 대기
	//
	// 이전 데이터가 아직 UI 서버에서 읽히지 않았다면
	// 여기서 대기함.
	//
	// sem[0] = EMPTY
	// sem[1] = FULL
	// ----------------------------------------------------
	sembuf waitEmpty{};

	waitEmpty.sem_num = 0;     // EMPTY
	waitEmpty.sem_op = -1;    // EMPTY: 1 -> 0
	waitEmpty.sem_flg = 0;

	timespec timeout{};
	timeout.tv_sec = 1;
	timeout.tv_nsec = 0;

	while (true)
	{
		if (semtimedop(sendSemId, &waitEmpty, 1, &timeout) == 0)
		{
			break;
		}

		if (errno == EINTR)
		{
			continue;
		}

		if (errno == EAGAIN)
		{
			std::cerr << "[ERROR] SIM_TO_NET write timeout\n";
			return false;
		}

		perror("semtimedop wait EMPTY");
		return false;
	}

	// ----------------------------------------------------
	// 2. 메시지 길이 저장
	//
	// pWriteShmAddr[0 ~ 3]
	// ----------------------------------------------------
	const std::uint32_t length = static_cast<std::uint32_t>(message.size());
	std::memcpy(pWriteShmAddr, &length,	LENGTH_SIZE);

	// ----------------------------------------------------
	// 3. 실제 메시지 저장
	//
	// pWriteShmAddr[4 ~]
	// ----------------------------------------------------
	if (length > 0)
	{
		std::memcpy(pWriteShmAddr + LENGTH_SIZE, message.data(), length);
	}

	// ----------------------------------------------------
	// 4. FULL 증가
	//
	// UI 서버에게 "공유메모리에 읽을 데이터가 있다"
	// 라고 알림
	// ----------------------------------------------------
	sembuf postFull{};

	postFull.sem_num = 1;      // FULL
	postFull.sem_op = +1;     // FULL: 0 -> 1
	postFull.sem_flg = 0;

	while (semop(sendSemId, &postFull, 1) == -1)
	{
		if (errno == EINTR)
		{
			continue;
		}

		perror("semop post FULL");
		return false;
	}

	std::cout << "[Simulation Send] " << message << '\n';

	return true;
}

void SharedMemControl::Wait()
{
	if (recvThreadStarted)
	{
		pthread_join(recvThread, nullptr);
		recvThreadStarted = false;
	}
}

void SharedMemControl::Stop()
{
	recvRunning.store(false);
}
