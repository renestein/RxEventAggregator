#pragma once
#include "../AppEventParticipant.h"

namespace TicketApp
{
  namespace Events
  {
    class PrintTicketResponseEvent;
    class PrintTicketRequestEvent;
    class RestartEvent;
    class PaymentReceivedEvent;
  }

  namespace Ui
  {
    class Display : public AppEventParticipant
    {
    public:
      using TicketType = int;
      using MyBase = AppEventParticipant;
      Display(const std::shared_ptr<EventProcessing::EventAggregator<EventProcessing::Event>>& messenger);

      void CancelButtonPressed();
      void SelectTicketButtonPressed(TicketType selectedTicket);
      void PaymentButtonPressed();
      void Start() override;


    protected:
      virtual void OnPaymentReceivedEvent(const Events::PaymentReceivedEvent& event);
      virtual void OnRestartEvent(const Events::RestartEvent& e);
      virtual void OnPrintTicketRequestEvent(const Events::PrintTicketRequestEvent& e);
      virtual void OnPrintTicketResponseEvent(const Events::PrintTicketResponseEvent& e);

      void ReleaseAllSubscriptions() override;
    private:
      bool _cancelButtonEnabled{false};
      bool _selectTicketButtonEnabled{false};
      bool _paymentButtonEnabled{false};
      rxcpp::composite_subscription _compositeSubscription;
    };
  }
}
