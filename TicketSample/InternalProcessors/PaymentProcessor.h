#pragma once
#include "../AppEventParticipant.h"
#include <memory>

namespace TicketApp
{
  namespace Events
  {
    class CanceledEvent;
    class PaymentRequestEvent;
    class TicketSelectedEvent;
    class RestartEvent;
  }

  namespace InternalProcessors
  {
    class PaymentProcessor : public AppEventParticipant
    {

    public:
      const int INVALID_AMOUNT = -1;
      using MyBase = AppEventParticipant;

      PaymentProcessor(const std::shared_ptr<EventProcessing::EventAggregator<EventProcessing::Event>>& messenger)
        : AppEventParticipant(messenger)
      {
      }

      void AddCoin(int amount);
      void Start() override;
    protected:

      virtual void OnRestartEvent(const Events::RestartEvent& event);
      virtual void OnTicketSelectedEvent(const Events::TicketSelectedEvent& event);
      virtual void OnPaymentRequestEvent(const Events::PaymentRequestEvent& event);
      virtual void OnCanceledEvent(const Events::CanceledEvent& event);
      void ReleaseAllSubscriptions() override;

    private:
      int _currentAmount = INVALID_AMOUNT;
      int _requiredAmount = INVALID_AMOUNT;
      rxcpp::composite_subscription _compositeSubscription;

      void publishPaymentReceived();
      void publishUnexpectedEvent() const;
      void clearAmounts();
      void returnMoney();
    };
  }
}
