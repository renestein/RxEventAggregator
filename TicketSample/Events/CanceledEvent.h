#pragma once
#include "EventProcessing/Event.h"

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