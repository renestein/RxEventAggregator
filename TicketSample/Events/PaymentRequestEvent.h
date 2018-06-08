#pragma once
#include "../../RXEventAggregator/Lib/EventProcessing/Event.h"

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
