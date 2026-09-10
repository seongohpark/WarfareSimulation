#include "SimulationTimeManager.h"

#include <ctime>
#include <iomanip>
#include <sstream>

double SimulationTimeManager::GetSimulationTimeSeconds() const
{
    std::lock_guard<std::mutex> lock(Mutex);

    return SimulationTimeSeconds;
}

uint64_t SimulationTimeManager::GetSimulationTimeMilliseconds() const
{
    std::lock_guard<std::mutex> lock(Mutex);

    return static_cast<uint64_t>(
        SimulationTimeSeconds * 1000.0);
}

double SimulationTimeManager::GetRealElapsedTimeSeconds() const
{
    std::lock_guard<std::mutex> lock(Mutex);

    if (State == ESimulationState::STOPPED)
    {
        return 0.0;
    }

    const auto now = SteadyClock::now();

    const std::chrono::duration<double> elapsed =
        now - RealStartTime;

    return elapsed.count();
}

std::string SimulationTimeManager::GetCurrentSystemTimeString() const
{
    const auto now = SystemClock::now();
    std::time_t currentTime = SystemClock::to_time_t(now);

    std::tm tm{};
    localtime_r(&currentTime, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}