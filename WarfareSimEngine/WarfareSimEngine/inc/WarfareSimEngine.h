#pragma once

#include <memory>

#include "SharedMemControl.h"

class WarfareSimEngine
{
public:
	static WarfareSimEngine* GetInstance();

	void Start();

	friend std::unique_ptr<WarfareSimEngine> std::make_unique<WarfareSimEngine>();
	friend std::unique_ptr<WarfareSimEngine>::deleter_type;

private:
	static std::unique_ptr<WarfareSimEngine> pWarfareSimEngine;
	std::unique_ptr<SharedMemControl> pSharedMemControl;

private:
	explicit WarfareSimEngine();
	virtual ~WarfareSimEngine();	
};