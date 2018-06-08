#include "stdafx.h"
#include "PrintTicketRequestEvent.h"
#include "EventTypes.h"
#include <boost/lexical_cast.hpp>

using namespace std;
namespace TicketApp
{
  namespace Events
  {
     const std::string PrintTicketRequestEvent::TICKET_SELECTED_EVENT_DESCRIPTION= "PrintTicketRequestEvent";
    PrintTicketRequestEvent::PrintTicketRequestEvent(TicketType ticketType): Event(APP_EVENT_PRINT_TICKET_REQUEST, TICKET_SELECTED_EVENT_DESCRIPTION),
                                                                             m_ticketType(ticketType)
    {

    }

    int PrintTicketRequestEvent::GetTicketType() const
    {
      return m_ticketType;
    }

    std::string PrintTicketRequestEvent::ToString() const
    {
      return Event::ToString()
             + " Ticket type: "
             + boost::lexical_cast<string>(m_ticketType);
    }
  }
}

