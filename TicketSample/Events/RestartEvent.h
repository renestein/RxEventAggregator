#pragma once
#include "../../RXEventAggregator/Lib/EventProcessing/Event.h"

namespace TicketApp
{
  namespace Events
  {
    class RestartEvent : public EventProcessing::Event
    {

    public:
      const static std::string RESTART_EVENT_DESCRIPTION;
      RestartEvent();
    
      private:

    };
  }
}
