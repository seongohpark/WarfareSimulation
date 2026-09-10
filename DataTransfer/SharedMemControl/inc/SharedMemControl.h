#pragma once

#include <sys/sem.h>
//#include "CTypeDef.h"

using std::string;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;
using std::stringstream;
using std::to_string;
using std::stoi;
using std::ios;

namespace CGA
{
	namespace DATATRANSFER
	{
		class CSharedMemControl
		{
		public:
			typedef struct
			{
				int READ_SHM_KEY;			// Read Shared Memory Key
				int WRITE_SHM_KEY;			// Write Shared Memory Key
				int READ_SHM_SIZE;			// Read Shared Memory Size
				int WRITE_SHM_SIZE;			// Write Shared Memory Size				
				int READ_SEMAPHORE_KEY;		// Read Semaphore Key
				int WRITE_SEMAPHORE_KEY;	// Write Semaphore Key
			}IPC_CONFIG;

			union semUni
			{
				int val;
				struct semid_ds* buf;
				unsigned short int* array;
			};

			// Member Variable
		public:
			IPC_CONFIG m_IpcConfig;			// 공유메모리 정보
			union semUni m_semUnion;
			int m_nReadShmID;				// 읽기용 공유메모리 ID
			char* m_pReadShmAddr;			// Attach된 Virtual Memory의 Address
			int m_nReadSemaphoreId;
			struct sembuf m_strtReadSemOpen;
			struct sembuf m_strtReadSemClose;

			int m_nWriteShmID;				// 쓰기용 공유메모리 ID
			char* m_pWriteShmAddr;
			int m_nWriteSemaphoreId;
			struct sembuf m_strtWriteSemOpen;
			struct sembuf m_strtWriteSemClose;			

		// Member Function
		public:
			explicit CSharedMemControl();
			virtual ~CSharedMemControl();

			void InitializeShm(void);			// 공유메모리 사용 관련 초기값 로딩

			void CreateReadShm(void);			// Check OK	// 읽기용 메모리 생성
			void CreateWriteShm(void);			// Check OK	// 쓰기용 메모리 생성
			void DeleteReadShm(void);			// Check OK // 읽기용 메모리 삭제
			void DeleteWriteShm(void);			// Check OK // 쓰기용 메모리 삭제

			int GetReadShm(char* str);			// Check OK
			void PutReadShm(const char* str);	//
			int GetWriteShm(char* str);			// Check OK
			void PutWriteShm(const char* str);	// Check OK
		};
	}
}