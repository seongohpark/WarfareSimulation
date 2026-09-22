#include "WarfareSimEngine.h"


std::unique_ptr<WarfareSimEngine> WarfareSimEngine::pWarfareSimEngine = nullptr;

WarfareSimEngine::WarfareSimEngine()
{
}

WarfareSimEngine::~WarfareSimEngine()
{
}

WarfareSimEngine* WarfareSimEngine::GetInstance()
{
	if (nullptr == pWarfareSimEngine)
	{
		pWarfareSimEngine = std::make_unique<WarfareSimEngine>();
	}

	return pWarfareSimEngine.get();
}

void WarfareSimEngine::Start()
{
	pSharedMemControl = std::make_unique<SharedMemControl>();
	pSharedMemControl->StartSharedMemProcess();
}