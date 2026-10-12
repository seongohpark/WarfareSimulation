#pragma once

#include "IGameControlListener.h"

class ICommonInterface : public IGameControlListener
{
public:
	virtual void OnRecvControlMessage(std::string recvMessage) {}
};