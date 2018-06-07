#pragma once
#include "TicketApp/AppEventParticipant.h"
#include "RStein.Common/CompositeDisposable.h"


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

      PaymentProcessor(const std::shared_ptr<::Schedulers::Scheduler>& scheduler,
        const std::shared_ptr<EventProcessing::Messenger>& messenger)
        : AppEventParticipant(scheduler, messenger)
      {
      }

      void AddCoin(int amount);

    protected:

      virtual void OnRestartEvent(const Events::RestartEvent& event);
      virtual void OnTicketSelectedEvent(const Events::TicketSelectedEvent& event);
      virtual void OnPaymentRequestEvent(const Events::PaymentRequestEvent& event);
      virtual void OnCanceledEvent(const Events::CanceledEvent& event);
      void OnStart() override;
      void ReleaseAllSubscriptions() override;

    private:
      int m_currentAmount = INVALID_AMOUNT;
      int m_requiredAmount = INVALID_AMOUNT;
      RStein::Utils::CompositeDisposablePtr m_subscriptions;

      void publishPaymentReceived();
      void publishUnexpectedEvent() const;
      void clearAmounts();
      void returnMoney();
    };
  }
}
