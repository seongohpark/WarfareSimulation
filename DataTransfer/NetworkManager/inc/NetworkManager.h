#pragma once

class NetworkManager
{
public:
    explicit NetworkManager();
    ~NetworkManager();

    void StartMasterServer();  // StartMasterServer

private:
    int ServerSocket;
};