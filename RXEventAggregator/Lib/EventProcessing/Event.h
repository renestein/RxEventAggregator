#pragma once
#include <string>
#include "Detail/DateTime.h"

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
    EventType _eventType;
    std::string _description;
    time_t _eventTime;
  };

  inline Event::Event(EventType eventType, std::string description) : _eventType(eventType),
                                                                 _description(std::move(description)),
                                                                 _eventTime(Detail::DateTime::GetCurrentDateTime())
    {
    }

  inline Event::EventType Event::GetEventType() const
    {
      return _eventType;
    }

  inline time_t Event::GetEventTime() const
    {
      return _eventTime;
    }

  inline std::string Event::ToString() const
    {
      return "EventId: " +
             std::to_string(_eventType) +
             " Description: " +
             _description;
    }
}
