#pragma once
#include "EventProcessing/Event.h"

namespace TicketApp
{
  namespace Events
  {
    class QuitEvent : public EventProcessing::Event
    {

    public:
      const static std::string DESCRIPTION;
      QuitEvent();
    
      private:

    };
  }
}