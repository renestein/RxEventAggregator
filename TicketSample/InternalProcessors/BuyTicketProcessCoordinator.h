#pragma once
#include "../AppEventParticipant.h"

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
      BuyTicketProcessCoordinator(const std::shared_ptr<EventProcessing::EventAggregator<EventProcessing::Event>>& messenger)
        : AppEventParticipant(messenger)
      {
      }

      void Start() override;
      void Stop() override;

    protected:
      virtual void OnCanceledEvent(Events::CanceledEvent& e);
      virtual void OnError(Events::ErrorEvent& e);
      void DisposeTimer();
      virtual void OnPaymentReceivedEvent(Events::PaymentReceivedEvent& e);
      virtual void OnTicketSelectedEvent(Events::TicketSelectedEvent& e);
      virtual void OnPrintTicketResponseEvent(Events::PrintTicketResponseEvent& e);
      void ReleaseAllSubscriptions() override;
    private:
      const int INVALID_TICKET = -1;
      void publishRestartEvent();
      void publishCanceledEvent();
      int _errorCount = 0;
      int _selectedTicket = INVALID_TICKET;
      rxcpp::composite_subscription _timerSubscription{};
      rxcpp::composite_subscription _compositeSubscription{};
    };

  }
}
