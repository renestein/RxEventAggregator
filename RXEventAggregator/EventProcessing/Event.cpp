#include "stdafx.h"
#include "Event.h"
#include "../Misc/DateTime.h"
using namespace std;
using namespace Misc;
namespace EventProcessing
{
  Event::Event(EventType eventType, std::string description) : m_eventType(eventType),
    m_description(std::move(description)),
    m_eventTime(DateTime::GetCurrentDateTime())
  {
  }

  Event::EventType Event::GetEventType() const
  {
    return m_eventType;
  }

  time_t Event::GetEventTime() const
  {
    return m_eventTime;
  }

  std::string Event::ToString() const
  {
    return "EventId: " +
      to_string(m_eventType) +
      " Description: " +
      m_description;
  }
}