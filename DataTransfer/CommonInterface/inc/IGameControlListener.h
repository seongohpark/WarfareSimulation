#pragma once

class IGameControlListener
{
public:
	virtual void OnRecvControlMessage(std::string recvMessage) = 0;
};

