#include "stdafx.h"
#include "BuyTicketProcessCoordinator.h"
#include "../../RXEventAggregator/Lib/EventProcessing/EventAggregator.h"
#include "../Events/RestartEvent.h"
#include "../Events/QuitEvent.h"
#include "../Events/CanceledEvent.h"
#include "../Events/PrintTicketRequestEvent.h"
#include "../Events/PrintTicketResponseEvent.h"
#include "../Events/PaymentReceivedEvent.h"
#include "../Events/ErrorEvent.h"
#include "../Events/TicketSelectedEvent.h"
#include <rxcpp/rx.hpp>
#include <memory>

using namespace std;
using namespace rxcpp;

namespace TicketApp
{
  namespace InternalProcessors
  {
    void BuyTicketProcessCoordinator::Start()
    {
      MyBase::Start();
      _compositeSubscription = GetMessenger()
                               .GetEventStream<Events::PrintTicketResponseEvent>().subscribe([this](
                                                                                 shared_ptr<Events::
                                                                                   PrintTicketResponseEvent>
                                                                                 printTicketResponseEvent)
                                                                                 {
                                                                                   OnPrintTicketResponseEvent(*printTicketResponseEvent);
                                                                                 });
      

      GetMessenger()
        .GetEventStream<Events::PaymentReceivedEvent>().subscribe(_compositeSubscription,
                                                                  [this](
                                                                  shared_ptr<Events::
                                                                    PaymentReceivedEvent>
                                                                  paymentReceivedEvent)
                                                                  {
                                                                    OnPaymentReceivedEvent(*paymentReceivedEvent);
                                                                  });
      GetMessenger()
        .GetEventStream<Events::TicketSelectedEvent>()
        .subscribe(_compositeSubscription, [this](
                   shared_ptr<Events::TicketSelectedEvent> ticketSelectedEvent)
                   {
                     OnTicketSelectedEvent(*ticketSelectedEvent);
                   });

      GetMessenger().GetEventStream<Events::CanceledEvent>()
                    .subscribe(_compositeSubscription,
                               [this](
                               shared_ptr<
                                 Events::
                                 CanceledEvent>
                               canceledEvent)
                               {
                                 OnCanceledEvent(*canceledEvent);
                               });

      GetMessenger()
        .GetEventStream<Events::ErrorEvent>()
        .subscribe(_compositeSubscription,
                   [this](
                   shared_ptr<Events::
                     ErrorEvent>
                   errorEvent)
                   {
                     OnError(*errorEvent);
                   });

      publishRestartEvent();
    }

    void BuyTicketProcessCoordinator::Stop()
    {
      MyBase::Stop();
      GetMessenger().PublishEvent(make_shared<Events::QuitEvent>());
    }

    void BuyTicketProcessCoordinator::publishCanceledEvent()
    {
      GetMessenger().PublishEvent(make_shared<Events::CanceledEvent>());
    }


    void BuyTicketProcessCoordinator::OnError(Events::ErrorEvent& e)
    {
      _errorCount++;
      publishRestartEvent();
    }

    void BuyTicketProcessCoordinator::DisposeTimer()
    {
      _timerSubscription.unsubscribe();
    }

    void BuyTicketProcessCoordinator::OnPaymentReceivedEvent(Events::PaymentReceivedEvent& e)
    {
      if (e.GetCurrentAmount() == e.GetRequiredAmount())
      {
        DisposeTimer();
        GetMessenger().PublishEvent(std::make_shared<Events::PrintTicketRequestEvent>(_selectedTicket));
      }
    }

    void BuyTicketProcessCoordinator::OnTicketSelectedEvent(Events::TicketSelectedEvent& e)
    {
      const int TIMEOUT_IN_SECONDS = 10;
      _selectedTicket = e.GetTicketType();

      _timerSubscription = observable<>::timer(std::chrono::seconds(TIMEOUT_IN_SECONDS)).subscribe([this](auto _)
      {
        publishCanceledEvent();
      });
    }

    void BuyTicketProcessCoordinator::publishRestartEvent()
    {
      DisposeTimer();
      _selectedTicket = INVALID_TICKET;
      GetMessenger().PublishEvent(make_shared<Events::RestartEvent>());
    }

    void BuyTicketProcessCoordinator::OnPrintTicketResponseEvent(Events::PrintTicketResponseEvent& e)
    {
      publishRestartEvent();
    }

    void BuyTicketProcessCoordinator::ReleaseAllSubscriptions()
    {
      DisposeTimer();
      _compositeSubscription.unsubscribe();
    }

    void BuyTicketProcessCoordinator::OnCanceledEvent(Events::CanceledEvent& e)
    {
      publishRestartEvent();
    }
  }
}
