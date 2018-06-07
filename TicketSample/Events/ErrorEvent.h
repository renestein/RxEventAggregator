#pragma once
#include "EventProcessing/Event.h"
#include <string>

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