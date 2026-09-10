#pragma once

class SimulationTimeManager
{
private:
	double SimulationTimeSeconds;	
	
public:
    SimulationTimeManager();
    
    double GetSimulationTimeSeconds() const;
	uint64_t GetSimulationTimeMilliseconds() const;
	double GetRealElapsedTimeSeconds() const;
	std::string GetCurrentSystemTimeString() const;
};