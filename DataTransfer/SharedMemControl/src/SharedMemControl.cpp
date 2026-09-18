#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>//#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "SharedMemControl.h"
#include "CommonUtil.h"

using std::string;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;
using std::stringstream;
using std::to_string;
using std::stoi;
using std::ios;

CSharedMemControl::CSharedMemControl()
	: m_IpcConfig{-1, -1 ,-1, -1, -1, -1}
	, m_semUnion{ -1 }
	, m_nReadShmID(-1)
	, m_pReadShmAddr()
	, m_nReadSemaphoreId(-1)
	, m_strtReadSemOpen{ 0, -1, SEM_UNDO }
	, m_strtReadSemClose{ 0, 1, SEM_UNDO }
	, m_nWriteShmID(-1)	
	, m_pWriteShmAddr()
	, m_nWriteSemaphoreId(-1)
	, m_strtWriteSemOpen{0, -1, SEM_UNDO}
	, m_strtWriteSemClose{0, 1, SEM_UNDO}	
{

}

CSharedMemControl::~CSharedMemControl()
{	
}

//------------------------------------------------------------------------
// 데이터전달처리 초기실행 시 공유메모리 설정값을 로드한다.
// ReadShm에 쓰고 WriteShm에서 읽는다.
//	
//
//------------------------------------------------------------------------
void CSharedMemControl::InitializeShm(void)
{
	string strConfigPath;
	strConfigPath.assign("../../cfg/ipc.cfg");

	string strKeyName = "SIM_TO_NET_SHM_KEY";
	string strSectionName = "SHARED_MEMORY_INFO";
	const string strWriteShmKey = CCommonUtility::GetValue(strKeyName, strSectionName, strConfigPath);

	strKeyName = "NET_TO_SIM_SHM_KEY";
	const string strReadShmKey = CCommonUtility::GetValue(strKeyName, strSectionName, strConfigPath);

	strKeyName = "SIM_TO_NET_SHM_SIZE";
	const string strReadShmSize = CCommonUtility::GetValue(strKeyName, strSectionName, strConfigPath);

	strKeyName = "NET_TO_SIM_SHM_SIZE";
	const string strWriteShmSize = CCommonUtility::GetValue(strKeyName, strSectionName, strConfigPath);

	strKeyName = "SIM_TO_NET_SEM_KEY";
	string strWriteSemKey = CCommonUtility::GetValue(strKeyName, strSectionName, strConfigPath);

	strKeyName = "NET_TO_SIM_SEM_KEY";
	const string strReadSemKey = CCommonUtility::GetValue(strKeyName, strSectionName, strConfigPath);

	m_IpcConfig.READ_SHM_KEY = stoi(strReadShmKey, nullptr);
	m_IpcConfig.WRITE_SHM_KEY = stoi(strWriteShmKey, nullptr);
	m_IpcConfig.READ_SHM_SIZE = stoi(strReadShmSize, nullptr);
	m_IpcConfig.WRITE_SHM_SIZE = stoi(strWriteShmSize, nullptr);
	m_IpcConfig.READ_SEMAPHORE_KEY = stoi(strReadSemKey, nullptr);
	m_IpcConfig.WRITE_SEMAPHORE_KEY = stoi(strWriteSemKey, nullptr);

	cout << "[INFO] " << __LINE__ << " - Config Path : " << strConfigPath << endl;
	cout << "[INFO] " << __LINE__ << " - SIM_TO_NET_SHM_KEY : " << m_IpcConfig.READ_SHM_KEY << endl;
	cout << "[INFO] " << __LINE__ << " - SIM_TO_NET_SHM_SIZE : " << m_IpcConfig.READ_SHM_SIZE << endl;
	cout << "[INFO] " << __LINE__ << " - SIM_TO_NET_SEM_KEY : " << m_IpcConfig.READ_SEMAPHORE_KEY << endl;

	cout << "[INFO] " << __LINE__ << " - NET_TO_SIM_SHM_KEY : " << m_IpcConfig.WRITE_SHM_KEY << endl;
	cout << "[INFO] " << __LINE__ << " - NET_TO_SIM_SHM_SIZE : " << m_IpcConfig.WRITE_SHM_SIZE << endl;
	cout << "[INFO] " << __LINE__ << " - NET_TO_SIM_SEM_KEY : " << m_IpcConfig.WRITE_SEMAPHORE_KEY << endl;
}

//------------------------------------------------------------------------
// CreateReadShm
// 
//	
//
//------------------------------------------------------------------------
void CSharedMemControl::CreateReadShm(void)
{
	// shmget : key값을 통해 Shared Memory의 id를 얻어온다.
	m_nReadShmID = shmget(static_cast<key_t>(m_IpcConfig.READ_SHM_KEY), m_IpcConfig.READ_SHM_SIZE, IPC_CREAT | 0666U);
	if (0 > m_nReadShmID)
	{
		cout << "[ERROR] Can't get read shared memory id : " << m_nReadShmID << endl;
	}

	//shmat : 프로세서에 공유메모리 세그먼트를 Attach한다.
	m_pReadShmAddr = static_cast<char*>(shmat(m_nReadShmID, nullptr, 0));	
	if (m_pReadShmAddr == reinterpret_cast<char*>(-1))
	{
		cout << "[ERROR] Can't attach shared memory" << endl;
	}

	m_nReadSemaphoreId = semget(static_cast<key_t>(m_IpcConfig.READ_SEMAPHORE_KEY), 1, IPC_CREAT | 0666U);
	m_semUnion.val = 1;
	const int nSemctlRslt = semctl(m_nReadSemaphoreId, 0, SETVAL, m_semUnion);
	if (0 > nSemctlRslt)
	{
		cout << "[ERROR] Semaphore SETVAL error" << endl;
	}
}

//------------------------------------------------------------------------
// CreateWriteShm 쓰기용 메모리 생성
// 
//	
//
//------------------------------------------------------------------------
void CSharedMemControl::CreateWriteShm(void)
{
	// shmget : key값을 통해 Shared Memory의 id를 얻어온다.
	m_nWriteShmID = shmget(static_cast<key_t>(m_IpcConfig.WRITE_SHM_KEY), m_IpcConfig.WRITE_SHM_SIZE, IPC_CREAT | 0666U);
	if (0 > m_nWriteShmID)
	{
		cout << "[ERROR] Can't get write shared memory id : " << m_nWriteShmID << endl;
	}

	//shmat : 프로세서에 공유메모리 세그먼트를 Attach한다.
	m_pWriteShmAddr = static_cast<char*>(shmat(m_nWriteShmID, nullptr, 0));	//shmat : 프로세서에 공유메모리 세그먼트를 Attach한다.
	if (m_pWriteShmAddr == reinterpret_cast<char*>(-1))
	{
		cout << "[ERROR] Can't attach shared memory" << endl;
	}

	m_nWriteSemaphoreId = semget(static_cast<key_t>(m_IpcConfig.WRITE_SEMAPHORE_KEY), 1, IPC_CREAT | 0666U);
	m_semUnion.val = 1;
	const int nSemctlRslt = semctl(m_nWriteSemaphoreId, 0, SETVAL, m_semUnion);
	if (0 > nSemctlRslt)
	{		
		cout << "[ERROR] Semaphore SETVAL error" << endl;
	}
}

//------------------------------------------------------------------------
// PutReadShm
// ReadShm에 쓰고 WriteShm에서 읽는다.
// 
//
//------------------------------------------------------------------------
void CSharedMemControl::PutReadShm(const char* InputData)
{
	if (InputData == nullptr ||	m_pReadShmAddr == nullptr || m_IpcConfig.READ_SHM_SIZE <= 0)
	{
		return;
	}

	if (m_nReadSemaphoreId < 0)
	{
		std::cerr << "[ERROR] Invalid read semaphore ID: " << m_nReadSemaphoreId << std::endl;
		return;
	}
	
	int SemaphoreResult;
	do
	{
		SemaphoreResult = semop(m_nReadSemaphoreId,	&m_strtReadSemOpen,	1);
	} while (SemaphoreResult == -1 && errno == EINTR);

	if (SemaphoreResult == -1)
	{
		std::cerr << "[ERROR] Failed to lock read semaphore: "	<< std::strerror(errno)	<< std::endl;
		return;
	}

	const std::size_t MemorySize = static_cast<std::size_t>(m_IpcConfig.READ_SHM_SIZE);

	// 마지막 한 바이트는 '\0'을 위해 남긴다.
	const std::size_t CopyLength =	std::min(std::strlen(InputData), MemorySize - 1);
	std::memset(m_pReadShmAddr,	0, MemorySize);
	
	// 실제 입력 데이터 기록
	if (CopyLength > 0)
	{
		std::memcpy(m_pReadShmAddr, InputData, CopyLength);
	}	
	m_pReadShmAddr[CopyLength] = '\0';
	
	do
	{
		SemaphoreResult = semop(m_nReadSemaphoreId,	&m_strtReadSemClose,1);
	} while (SemaphoreResult == -1 && errno == EINTR);
	
	if (SemaphoreResult == -1)
	{
		std::cerr << "[ERROR] Failed to unlock read semaphore: " << std::strerror(errno) << std::endl;
	}
}

//------------------------------------------------------------------------
// Description : 전투모의체계에서 송신하는 데이터를 읽는다.
// 
//	
//
//------------------------------------------------------------------------
int CSharedMemControl::GetWriteShm(char* OutBuffer,	std::size_t OutBufferSize)
{
	if (OutBuffer == nullptr || OutBufferSize == 0)
	{
		std::cerr << "[ERROR] Invalid output buffer" << std::endl;
		return -1;
	}

	// 실패하거나 데이터가 없을 경우에도 안전하게 빈 문자열로 유지한다.
	OutBuffer[0] = '\0';

	if (m_pWriteShmAddr == nullptr)
	{
		std::cerr << "[ERROR] Write shared memory is not attached" << std::endl;
		return -1;
	}

	if (m_IpcConfig.WRITE_SHM_SIZE <= 0)
	{
		std::cerr << "[ERROR] Invalid WRITE_SHM_SIZE: " << m_IpcConfig.WRITE_SHM_SIZE << std::endl;
		return -1;
	}

	if (m_nWriteSemaphoreId < 0)
	{
		std::cerr << "[ERROR] Invalid write semaphore ID" << std::endl;
		return -1;
	}

	// 공유 메모리 접근 잠금
	int SemaphoreResult;
	do
	{
		SemaphoreResult = semop(m_nWriteSemaphoreId, &m_strtWriteSemOpen, 1);
	} while (SemaphoreResult == -1 && errno == EINTR);

	if (SemaphoreResult == -1)
	{
		std::cerr << "[ERROR] Failed to lock write semaphore: "	<< std::strerror(errno)	<< std::endl;
		return -1;
	}

	const std::size_t SharedMemorySize = static_cast<std::size_t>(m_IpcConfig.WRITE_SHM_SIZE);

	// 공유 메모리 범위 내에서 문자열 길이 검색
	// 공유 메모리에 '\0'이 없더라도 범위를 넘지 않는다.
	std::size_t DataLength = 0;
	while (DataLength < SharedMemorySize &&	m_pWriteShmAddr[DataLength] != '\0')
	{
		++DataLength;
	}

	// 출력 버퍼 마지막 한 바이트는 '\0'을 위해 남긴다.
	const std::size_t CopyLength = std::min(DataLength,	OutBufferSize - 1);
	if (CopyLength > 0)
	{
		std::memcpy(OutBuffer, m_pWriteShmAddr, CopyLength);
	}
	// 반드시 널 종료 문자열로 만든다.
	OutBuffer[CopyLength] = '\0';

	// 읽은 데이터 제거
	std::memset(m_pWriteShmAddr, 0,	SharedMemorySize);

	// 공유 메모리 접근 잠금 해제
	do
	{
		SemaphoreResult = semop(m_nWriteSemaphoreId, &m_strtWriteSemClose, 1);
	} while (SemaphoreResult == -1 && errno == EINTR);

	if (SemaphoreResult == -1)
	{
		std::cerr << "[ERROR] Failed to unlock write semaphore: " << std::strerror(errno) << std::endl;
		return -1;
	}
	return static_cast<int>(CopyLength);
}
