#pragma once
#include "TicketApp/AppEventParticipant.h"
#include "RStein.Common/CompositeDisposable.h"

namespace TicketApp
{
  namespace Events
  {
    class TicketSelectedEvent;
    class ErrorEvent;
    class PrintTicketResponseEvent;
    class PaymentReceivedEvent;
    class CanceledEvent;
  }

  namespace InternalProcessors
  {
    class BuyTicketProcessCoordinator : public AppEventParticipant
    {
    public:
      using MyBase = AppEventParticipant;
      BuyTicketProcessCoordinator(const std::shared_ptr<::Schedulers::Scheduler>& scheduler,
        const std::shared_ptr<EventProcessing::Messenger>& messenger)
        : AppEventParticipant(scheduler, messenger)
      {
      }

    protected:
      void OnStart() override;
      void OnStop() override;
      virtual void OnCanceledEvent(Events::CanceledEvent& e);
      virtual void OnErrorEvent(Events::ErrorEvent& e);
      void DisposeTimer();
      virtual void OnPaymentReceivedEvent(Events::PaymentReceivedEvent& e);
      virtual void OnTicketSelectedEvent(Events::TicketSelectedEvent& e);
      virtual void OnPrintTicketResponseEvent(Events::PrintTicketResponseEvent& e);
      void ReleaseAllSubscriptions() override;
    private:
      const int INVALID_TICKET = -1;
      RStein::Utils::CompositeDisposablePtr m_subscriptionsDisposable;
      RStein::Utils::DisposablePtr m_timerDisposable;
      void publishRestartEvent();
      void publishCanceledEvent();
      int m_errorCount;
      int m_selectedTicket = INVALID_TICKET;
    };

  }
}
