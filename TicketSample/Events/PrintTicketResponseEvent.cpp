#include "stdafx.h"
#include "EventTypes.h"
#include "PrintTicketResponseEvent.h"

namespace TicketApp
{
  namespace Events
  {
     const std::string PrintTicketResponseEvent::DESCRIPTION = "PrintTicketResponseEvent";
    PrintTicketResponseEvent::PrintTicketResponseEvent() : Event(APP_EVENT_PRINT_TICKET_RESPONSE, DESCRIPTION)
    {

    }
  }
}
