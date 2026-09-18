#include <iostream>
#include <sys/socket.h>	
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <memory>

#include "NetworkManager.h"
#include "CommonUtil.h"

void* StartDataTrsfMasterRecvProcess(void* arg);

NetworkManager::NetworkManager()
	: ServerSocket(-1)
{
}


NetworkManager::~NetworkManager()
{   
}

void NetworkManager::StartMasterServer()
{
	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in strtServerAddr;
	memset(&strtServerAddr, 0x00U, sizeof(strtServerAddr));

	strtServerAddr.sin_family = AF_INET;
	uint32_t addr_v = static_cast<uint32_t>(htonl(INADDR_ANY));
	strtServerAddr.sin_addr.s_addr = addr_v;

	std::string strConfigPath;
	strConfigPath.assign("./Resource/sysconfig.cfg");

	std::string strKeyName = "PORT";
	const std::string strSectionName = "DATATRANSFER_SERVER_INFO";
	const std::string strPortNo = CCommonUtility::GetValue(strConfigPath, strSectionName, strKeyName);
	strtServerAddr.sin_port = htons(stoi(strPortNo, nullptr));
	
	strKeyName = "BUFFER_SIZE";
	std::string strBufferSize = CCommonUtility::GetValue(strConfigPath, strSectionName, strKeyName);
	int nSockBufSize = std::stoi(strBufferSize);
	
	socklen_t sockLen = sizeof(nSockBufSize);
	setsockopt(ServerSocket, SOL_SOCKET, SO_SNDBUF, static_cast<void*>(&nSockBufSize), sockLen);

	const int nSocketOpt = 1;
	setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &nSocketOpt, sizeof(nSocketOpt));

	bind(ServerSocket, reinterpret_cast<struct sockaddr*>(&strtServerAddr), sizeof(strtServerAddr));

	strKeyName = "MAX_BACKLOG";
	std::string strMaxBackLog = CCommonUtility::GetValue(strConfigPath, strSectionName, strKeyName);
	int nMaxBackLog = std::stoi(strMaxBackLog);

	listen(ServerSocket, nMaxBackLog);	//int nListenResult = 

	struct sockaddr_in strtExternalAddr;
	int nSlaveSocket = 0;
	int nClientAddrLength = sizeof(strtExternalAddr);

	while (true)
	{
		nSlaveSocket = accept(ServerSocket, reinterpret_cast<struct sockaddr*>(&strtExternalAddr), reinterpret_cast<socklen_t*>(&nClientAddrLength));
		if (nSlaveSocket != -1)
		{
			char hostname[128];
			gethostname(hostname, sizeof(hostname));
			
			struct addrinfo tmp_hints;
			memset(&tmp_hints, 0 , sizeof(tmp_hints));
			tmp_hints.ai_family = AF_INET;
			
			struct addrinfo *info = nullptr;
			getaddrinfo(&hostname[0], nullptr, &tmp_hints, &info);
			
			struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in *>(info->ai_addr);
			
			char szIpAddress[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(ipv4->sin_addr), szIpAddress, INET_ADDRSTRLEN);
			std::string strIpAddress(szIpAddress);
			std::cout << "[DEBUG] connect : " << strIpAddress << std::endl;

			int* pSlaveSocket = new int(nSlaveSocket);
			
			pthread_t threadId = 0;
			const int ret = pthread_create(&threadId, nullptr, StartDataTrsfMasterRecvProcess, pSlaveSocket);
			if (ret != 0)
			{
				std::cerr << "[ERROR] pthread_create() failed: " << std::strerror(ret) << std::endl;

				delete pSlaveSocket;
				close(nSlaveSocket);
			}
		}
	}
}

void* StartDataTrsfMasterRecvProcess(void* arg)
{
	std::cout << "[DEBUG] StartDataTrsfMasterRecvProcess" << std::endl;
	
	std::unique_ptr<int> socketHolder(static_cast<int*>(arg));
	const int slaveSocket = *socketHolder;

	std::string strConfigPath;
	strConfigPath.assign("./Resource/sysconfig.cfg");
	std::string strKeyName = "BUFFER_SIZE";
	const std::string strSectionName = "DATATRANSFER_SERVER_INFO";
	std::string strBufferSize = CCommonUtility::GetValue(strConfigPath, strSectionName, strKeyName);
	int nSockBufSize = std::stoi(strBufferSize);

	std::vector<char> vecReadBuffer(nSockBufSize);

	while (true)
	{
		const ssize_t readLength = recv(slaveSocket, vecReadBuffer.data(), vecReadBuffer.size(), 0);
	}


	return nullptr;
}