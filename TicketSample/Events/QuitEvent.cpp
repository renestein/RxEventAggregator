#include "stdafx.h"
#include "EventTypes.h"
#include "QuitEvent.h"

namespace TicketApp
{
  namespace Events
  {
    const std::string QuitEvent::DESCRIPTION = "Application QUIT event";

    QuitEvent::QuitEvent() : Event(APP_EVENT_QUIT, DESCRIPTION)
    {
    }
  }
}
