#pragma once

#include "ISimulationControl.h"

class SimulationControlManager : public ISimulationControl
{
public:
    SimulationControlManager();
    virtual ~SimulationControlManager();

    virtual bool Start() override;
    virtual bool Pause() override;
    virtual bool Resume() override;
    virtual bool Stop() override;

private:
    mutable std::mutex SimStateMutex;
    SimulationState SimState;
};