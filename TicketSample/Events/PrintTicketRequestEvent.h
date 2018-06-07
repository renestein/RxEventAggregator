#pragma once
#include "EventProcessing/Event.h"

namespace TicketApp
{
  namespace Events
  {
    class PrintTicketRequestEvent : public EventProcessing::Event
    {
      using TicketType = int;
    public:
      const static std::string TICKET_SELECTED_EVENT_DESCRIPTION;

      PrintTicketRequestEvent(TicketType ticketType);
      int GetTicketType() const;
      std::string ToString() const override;
    private:
      int m_ticketType;
    };
  }
}
