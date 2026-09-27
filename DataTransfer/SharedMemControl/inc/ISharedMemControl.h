#pragma once

class ISharedMemControl
{
public:
	virtual void RecvSharedMemMessage() = 0;
	virtual bool SendSharedMemMessage(const std::string& message) = 0;
};