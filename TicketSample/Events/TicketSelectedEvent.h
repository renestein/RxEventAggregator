#pragma once
#include "EventProcessing/Event.h"

namespace TicketApp
{
  namespace Events
  {
    class TicketSelectedEvent : public EventProcessing::Event
    {
      using TicketType = int;
    public:
      const static std::string TICKET_SELECTED_EVENT_DESCRIPTION;

      TicketSelectedEvent(TicketType ticketType);
      int GetTicketType() const;
      std::string ToString() const override;
    private:
      int m_ticketType;
    };
  }
}
