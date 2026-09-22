#include <unistd.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <limits.h>
#include <filesystem>

#include "SharedMemControl.h"
#include "CommonUtil.h"

SharedMemControl::SharedMemControl()
	: pWriteShmAddr(nullptr)
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

	// Engine Write
	std::string strSimToNetKey = "SIM_TO_NET_SHM_KEY";
	const int nSimToNetKey = std::stoi(CCommonUtility::GetValue(strConfigPath, strSectionName, strSimToNetKey));

	std::string strSimToNetSize = "SIM_TO_NET_SHM_SIZE";
	const int nSimToNetSize = std::stoi(CCommonUtility::GetValue(strConfigPath, strSectionName, strSimToNetSize));

	// SharedMemory Create
	const int nSimToNetId = shmget(static_cast<key_t>(nSimToNetKey), nSimToNetSize, IPC_CREAT | 0666U);
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

	pWriteShmAddr = static_cast<char*>(pAddr);
}
