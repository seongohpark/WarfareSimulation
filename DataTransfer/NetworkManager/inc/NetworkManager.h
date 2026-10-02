#pragma once

class IGameControlListener;
struct MasterRecvContext
{
    int slaveSocket;
    IGameControlListener* listener;
};

class NetworkManager
{
public:
    explicit NetworkManager();
    ~NetworkManager();

    void StartMasterServer(IGameControlListener* pListener);

private:
    int ServerSocket;
    IGameControlListener* pGameControlListener;

    static void* StartDataTrsfMasterRecvProcess(void* arg);
};