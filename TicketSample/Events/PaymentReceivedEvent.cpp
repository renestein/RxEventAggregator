#include "stdafx.h"
#include "PaymentReceivedEvent.h"
#include "EventTypes.h"
#include <boost/lexical_cast.hpp>
#include <string>
using namespace std;
namespace TicketApp
{
  namespace Events
  {
    const string PaymentReceivedEvent::PAYMENT_RECEIVED_EVENT_DESCRIPTION = "PaymentReceivedEvent";

    PaymentReceivedEvent::
    PaymentReceivedEvent(int currentAmount, int requiredAmount) : Event(APP_EVENT_PAYMENT_RECEIVED,
                                                                        PAYMENT_RECEIVED_EVENT_DESCRIPTION),
                                                                  m_currentAmount(currentAmount),
                                                                  m_requiredAmount(requiredAmount)
    {
    }

    int PaymentReceivedEvent::GetCurrentAmount() const
    {
      return m_currentAmount;
    }

    int PaymentReceivedEvent::GetRequiredAmount() const
    {
      return m_requiredAmount;
    }

    std::string PaymentReceivedEvent::ToString() const
    {
      return Event::ToString() + " CurrentAmount: " 
                               + boost::lexical_cast<string>(m_currentAmount)
                               + " RequiredAmount: "
                               + boost::lexical_cast<string>(m_requiredAmount);
    }
  }
}
