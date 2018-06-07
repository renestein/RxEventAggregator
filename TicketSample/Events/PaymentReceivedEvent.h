#pragma once
#include "EventProcessing/Event.h"

namespace TicketApp
{
  namespace Events
  {
    class PaymentReceivedEvent : public EventProcessing::Event
    {
    public:
      const static std::string PAYMENT_RECEIVED_EVENT_DESCRIPTION;

      PaymentReceivedEvent(int currentAmount, int requiredAmount);
     int GetCurrentAmount() const;
      int GetRequiredAmount() const;

      std::string ToString() const override;
    private:
      int m_currentAmount;
      int m_requiredAmount;
    };
  }
}
