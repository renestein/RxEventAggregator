#pragma once
#include <string>
#include "../../RXEventAggregator/Lib/EventProcessing/Event.h"

namespace TicketApp
{
  namespace Events
  {
    class ErrorEvent : public EventProcessing::Event
    {

    public:
      const static std::string DESCRIPTION;
      explicit ErrorEvent(const std::string &description);
    
      private:

    };
  }
}
