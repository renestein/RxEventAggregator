#pragma once
#include "EventProcessing/Event.h"

namespace TicketApp
{
  namespace Events
  {
    class PrintTicketResponseEvent : public EventProcessing::Event
    {

    public:
      const static std::string DESCRIPTION;
      PrintTicketResponseEvent();
    
      private:

    };
  }
}