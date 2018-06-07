﻿#pragma once
#include "TicketApp/AppEventParticipant.h"
#include "RStein.Common/CompositeDisposable.h"

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
      Display(const std::shared_ptr<::Schedulers::Scheduler>& scheduler,
              const std::shared_ptr<EventProcessing::Messenger>& messenger);

      void CancelButtonPressed();
      void SelectTicketButtonPressed(TicketType selectedTicket);
      void PaymentButtonPressed();


    protected:
      void OnStart() override;
      virtual void OnPaymentReceivedEvent(const Events::PaymentReceivedEvent& event);
      virtual void OnRestartEvent(const Events::RestartEvent& e);
      virtual void OnPrintTicketRequestEvent(const Events::PrintTicketRequestEvent& e);
      virtual void OnPrintTicketResponseEvent(const Events::PrintTicketResponseEvent& e);

      void ReleaseAllSubscriptions() override;
    private:
      bool m_cancelButtonEnabled{false};
      bool m_selectTicketButtonEnabled{false};
      bool m_paymentButtonEnabled{false};
      RStein::Utils::CompositeDisposablePtr m_compositeDisposable;
    };
  }
}
