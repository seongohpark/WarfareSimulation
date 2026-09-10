#pragma once

#include <mutex>
#include <queue>
#include <unordered_map>
#include <vector>
#include <functional>

enum class EventType : uint32_t
{
    None = 0,

    // Simulation
    SimulationStart,
    SimulationPause,
    SimulationResume,
    SimulationStop,

    // Entity
    EntityCreated,
    EntityRemoved,
    EntityMoved,

    // Combat
    WeaponFired,
    EntityHit,
    EntityDamaged,
    EntityDestroyed,

    // Sensor
    EntityDetected,
    EntityIdentified,
    EntityLost,

    // Command
    CommandReceived
};

struct Event
{
    uint64_t EventId = 0;
    EventType Type = EventType::None;
    double SimulationTime = 0.0;
    uint64_t SourceEntityId = 0;
    uint64_t TargetEntityId = 0;
};

class EventManager
{
public:
    using EventCallback = std::function<void(const Event&)>;

public:
    EventManager();
    ~EventManager();

	void PushEvent(const Event& event);
    void ProcessEvents();

    void Subscribe(EventType eventType, EventCallback callback);
    void Clear();

    size_t GetPendingEventCount();

private:
    void DispatchEvent(const Event& event);

private:
	std::mutex EventMutex;
    std::mutex SubscriberMutex;
	std::queue<Event> EventQueue;
    std::unordered_map<EventType, std::vector<EventCallback>> Subscribers;
	uint64_t NextEventId = 1;
};