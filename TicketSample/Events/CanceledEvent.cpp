#include "stdafx.h"
#include "EventTypes.h"
#include "CanceledEvent.h"

namespace TicketApp
{
  namespace Events
  {
     const std::string CanceledEvent::DESCRIPTION = "CanceledEvent";
    CanceledEvent::CanceledEvent() : Event(APP_EVENT_CANCELED, DESCRIPTION)
    {

    }
  }
}
