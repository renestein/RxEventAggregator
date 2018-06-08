#include "stdafx.h"
#include "RestartEvent.h"
#include "EventTypes.h"

namespace TicketApp
{
  namespace Events
  {
     const std::string RestartEvent::RESTART_EVENT_DESCRIPTION = "RestartEvent";
    RestartEvent::RestartEvent() : Event(APP_EVENT_RESTART, RESTART_EVENT_DESCRIPTION)
    {

    }
  }
}
