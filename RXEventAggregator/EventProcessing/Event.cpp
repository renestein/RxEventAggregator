#include "stdafx.h"
#include "Event.h"
#include "../Misc/DateTime.h"
using namespace std;
using namespace Misc;

namespace RxEventAggregator
{
  namespace EventProcessing
  {
    Event::Event(EventType eventType, std::string description) : _eventType(eventType),
                                                                 _description(std::move(description)),
                                                                 _eventTime(DateTime::GetCurrentDateTime())
    {
    }

    Event::EventType Event::GetEventType() const
    {
      return _eventType;
    }

    time_t Event::GetEventTime() const
    {
      return _eventTime;
    }

    std::string Event::ToString() const
    {
      return "EventId: " +
             to_string(_eventType) +
             " Description: " +
             _description;
    }
  }
}
