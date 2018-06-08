#include "stdafx.h"
#include "TicketSelectedEvent.h"
#include "EventTypes.h"
#include <boost/lexical_cast.hpp>

using namespace std;
namespace TicketApp
{
  namespace Events
  {
    const std::string TicketSelectedEvent::TICKET_SELECTED_EVENT_DESCRIPTION = "TicketSelectedEvent";
    TicketSelectedEvent::TicketSelectedEvent(TicketType ticketType) : Event(APP_EVENT_TICKET_SELECTED, TICKET_SELECTED_EVENT_DESCRIPTION),
      m_ticketType{ ticketType }
    {

    }

    int TicketSelectedEvent::GetTicketType() const
    {
      return m_ticketType;
    }

    std::string TicketSelectedEvent::ToString() const
    {
      return Event::ToString()
        + " Ticket type: "
        + boost::lexical_cast<string>(m_ticketType);
    }
  }
}

