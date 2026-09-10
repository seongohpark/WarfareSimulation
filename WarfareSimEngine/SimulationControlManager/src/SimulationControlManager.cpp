#include "SimulationControlManager.h"

SimulationControlManager::SimulationControlManager()
	: SimState(SimulationState::STOPPED)
{

}

SimulationControlManager::~SimulationControlManager()
{

}

bool SimulationControlManager::Start()
{
	bool bReturn = false;
	std::lock_guard<std::mutex> lock(SimStateMutex);
	if (SimState != SimulationState::STOPPED)
	{
		bReturn = false;
	}
	else
	{
		// TODO:

		SimState = SimulationState::RUNNING;
		bReturn = true;
	}

	return bReturn;
}

bool SimulationControlManager::Pause()
{
	bool bReturn = false;
	std::lock_guard<std::mutex> lock(SimStateMutex);
	if (SimState != SimulationState::RUNNING)
	{
		bReturn = false;
	}
	else
	{
		// TODO:

		SimState = SimulationState::PAUSED;
		bReturn = true;
	}

	return bReturn;
}

bool SimulationControlManager::Resume()
{
	bool bReturn = false;
	std::lock_guard<std::mutex> lock(SimStateMutex);
	if (SimState != SimulationState::PAUSED)
	{
		bReturn = false;
	}
	else
	{
		// TODO:

		SimState = SimulationState::RUNNING;
		bReturn = true;
	}

	return bReturn;
}

bool SimulationControlManager::Stop()
{
	bool bReturn = false;
	std::lock_guard<std::mutex> lock(SimStateMutex);
	if (SimState == SimulationState::STOPPED)
	{
		bReturn = false;
	}
	else
	{
		// TODO:

		SimState = SimulationState::STOPPED;
		bReturn = true;
	}

	return bReturn;
}