#pragma once
#include "../../RXEventAggregator/Lib/EventProcessing/Event.h"

namespace TicketApp
{
  namespace Events
  {
    class CanceledEvent : public EventProcessing::Event
    {

    public:
      const static std::string DESCRIPTION;
      CanceledEvent();
    
      private:

    };
  }
}
