#pragma once
#include <string>
namespace EventProcessing
{
  class Event
  {
  public:
     using EventType = unsigned long long;
    explicit Event(EventType eventType, std::string description = "");
    virtual ~Event() = default;
    Event(const Event& other) = delete;
    Event(Event&& other) noexcept = delete;
    Event& operator=(const Event& other) = delete;
    Event& operator=(Event&& other) noexcept = delete;
    EventType GetEventType() const;
    time_t GetEventTime() const;
    virtual std::string ToString() const;

  private:
    EventType m_eventType;
    std::string m_description;
    time_t m_eventTime;
  };
}
