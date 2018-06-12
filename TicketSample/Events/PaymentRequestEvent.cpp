#include "stdafx.h"
#include "PaymentRequestEvent.h"
#include "EventTypes.h"

namespace TicketApp
{
  namespace Events
  {
     const std::string PaymentRequestEvent::DESCRIPTION = "PaymentRequestEvent";
    PaymentRequestEvent::PaymentRequestEvent() : Event(APP_EVENT_PAYMENT_REQUEST, DESCRIPTION)
    {

    }
  }
}
