#pragma once
#include "../../RXEventAggregator/Lib/EventProcessing/Event.h"

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
