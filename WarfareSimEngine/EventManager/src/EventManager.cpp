#include "EventManager.h"

EventManager::EventManager()
{
}

EventManager::~EventManager()
{    
}

void EventManager::PushEvent(const Event& inputEvent)
{
    std::lock_guard<std::mutex> lock(EventMutex);

    Event event = inputEvent;
    event.EventId = NextEventId++;
    EventQueue.push(event);
}

void EventManager::ProcessEvents()
{
    while (true)
    {
        Event event;
        {
            std::lock_guard<std::mutex> lock(EventMutex);

            if (EventQueue.empty())
            {
                break;
            }

            event = EventQueue.front();

            EventQueue.pop();
        }

        //DispatchEvent(event);
    }
}

void EventManager::Subscribe(EventType eventType, EventCallback callback)
{
    if (!callback)
    {
        return;
    }

    std::lock_guard<std::mutex> lock(SubscriberMutex);
    Subscribers[eventType].push_back(std::move(callback));
}

void EventManager::DispatchEvent(const Event& event)
{
    std::vector<EventCallback> callbacks;
    {
        std::lock_guard<std::mutex> lock(SubscriberMutex);
        auto iter = Subscribers.find(event.Type);
        if (iter == Subscribers.end())
        {
            return;
        }

        callbacks = iter->second;
    }

    for (auto& callback : callbacks)
    {
        if (callback)
        {
            callback(event);
        }
    }
}

void EventManager::Clear()
{
    {
        std::lock_guard<std::mutex> lock(EventMutex);
        while (!EventQueue.empty())
        {
            EventQueue.pop();
        }
    }

    {
        std::lock_guard<std::mutex> lock(SubscriberMutex);
        Subscribers.clear();
    }
}

size_t EventManager::GetPendingEventCount()
{
    std::lock_guard<std::mutex> lock(EventMutex);
    return EventQueue.size();
}