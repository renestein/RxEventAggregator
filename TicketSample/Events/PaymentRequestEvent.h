#pragma once
#include "EventProcessing/Event.h"

namespace TicketApp
{
  namespace Events
  {
    class PaymentRequestEvent : public EventProcessing::Event
    {

    public:
      const static std::string DESCRIPTION;
      PaymentRequestEvent();
    
      private:

    };
  }
}