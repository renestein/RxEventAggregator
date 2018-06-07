#include "stdafx.h"
#include "BuyTicketProcessCoordinator.h"
#include "EventProcessing/Messenger.h"
#include <memory>
#include "TicketApp/Events/RestartEvent.h"
#include "TicketApp/Events/PaymentReceivedEvent.h"
#include "TicketApp/Events/CanceledEvent.h"
#include "TicketApp/Events/ErrorEvent.h"
#include "TicketApp/Events/PrintTicketRequestEvent.h"
#include "TicketApp/Events/PrintTicketResponseEvent.h"
#include "TicketApp/Events/TicketSelectedEvent.h"
#include "TicketApp/Events/QuitEvent.h"
#include "Schedulers/TimerDescriptor.h"
using namespace std;

namespace TicketApp
{
  namespace InternalProcessors
  {
    void BuyTicketProcessCoordinator::OnStart()
    {
      MyBase::OnStart();
      auto printResponseEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, Events::PrintTicketResponseEvent>(shared_from_this(),
                                                            [this](
                                                            shared_ptr<Events::PrintTicketResponseEvent>
                                                            printTicketResponseEvent)
                                                            {
                                                              ScheduleFutureOperation([printTicketResponseEvent, this]
                                                                                      {
                                                                                        OnPrintTicketResponseEvent(*printTicketResponseEvent);
                                                                                      },
                                                                                      "BuyTicketProcessCoordinator.OnPrintTicketResponseEvent");
                                                            });


      auto paymentEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, Events::PaymentReceivedEvent>(shared_from_this(),
                                                        [this](
                                                        shared_ptr<Events::PaymentReceivedEvent> paymentReceivedEvent)
                                                        {
                                                          ScheduleFutureOperation([paymentReceivedEvent, this]
                                                          {
                                                            OnPaymentReceivedEvent(*paymentReceivedEvent);
                                                          }, "BuyTicketProcessCoordinator.OnPaymentReceived");
                                                        });

      auto ticketSelectedEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, Events::TicketSelectedEvent>(shared_from_this(),
                                                       [this](
                                                       shared_ptr<Events::TicketSelectedEvent> ticketSelectedEvent)
                                                       {
                                                         ScheduleFutureOperation([ticketSelectedEvent, this]
                                                         {
                                                           OnTicketSelectedEvent(*ticketSelectedEvent);
                                                         }, "BuyTicketProcessCoordinator.OnTicketSelectedEvent");
                                                       });

      auto canceledEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, Events::CanceledEvent>(shared_from_this(),
                                                 [this](shared_ptr<Events::CanceledEvent> canceledEvent)
                                                 {
                                                   ScheduleFutureOperation([canceledEvent, this]
                                                   {
                                                     OnCanceledEvent(*canceledEvent);
                                                   }, "BuyTicketProcessCoordinator.OnCanceledEvent");
                                                 });

      auto errorEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, Events::ErrorEvent>(shared_from_this(),
                                              [this](shared_ptr<Events::ErrorEvent> errorEvent)
                                              {
                                                ScheduleFutureOperation([errorEvent, this]
                                                {
                                                  OnErrorEvent(*errorEvent);
                                                }, "BuyTicketProcessCoordinator.OnErrorEvent");
                                              });

      m_subscriptionsDisposable = make_shared<RStein::Utils::CompositeDisposableResource>();
      m_subscriptionsDisposable->registerDisposable(printResponseEventSubscription);
      m_subscriptionsDisposable->registerDisposable(paymentEventSubscription);
      m_subscriptionsDisposable->registerDisposable(ticketSelectedEventSubscription);
      m_subscriptionsDisposable->registerDisposable(canceledEventSubscription);
      m_subscriptionsDisposable->registerDisposable(errorEventSubscription);
      publishRestartEvent();
    }

    void BuyTicketProcessCoordinator::OnStop()
    {
      MyBase::OnStop();
       GetMessenger().PublishEvent(make_shared<Events::QuitEvent>());
    }

    void BuyTicketProcessCoordinator::publishCanceledEvent()
    {
      GetMessenger().PublishEvent(make_shared<Events::CanceledEvent>());
    }

 
    void BuyTicketProcessCoordinator::OnErrorEvent(Events::ErrorEvent& e)
    {
      m_errorCount++;
      publishRestartEvent();
    }

    void BuyTicketProcessCoordinator::DisposeTimer()
    {
      if (m_timerDisposable)
      {
        m_timerDisposable->Dispose();
        m_timerDisposable.reset();
      }
    }

    void BuyTicketProcessCoordinator::OnPaymentReceivedEvent(Events::PaymentReceivedEvent& e)
    {
      if (e.GetCurrentAmount() == e.GetRequiredAmount())
      {
        DisposeTimer();
        GetMessenger().PublishEvent(std::make_shared<Events::PrintTicketRequestEvent>(m_selectedTicket));
      }
    }

    void BuyTicketProcessCoordinator::OnTicketSelectedEvent(Events::TicketSelectedEvent& e)
    {
      m_selectedTicket = e.GetTicketType();
      auto timer = make_shared<Schedulers::TimerDescriptor>(boost::posix_time::seconds(10),
                                                            [this, sharedThis = shared_from_this()](
                                                            const boost::system::error_code& error,
                                                            const std::shared_ptr<Schedulers::TimerDescriptor>&
                                                            desriptor)
                                                            {
                                                              publishCanceledEvent();
                                                            },
                                                            nullptr,
                                                            [this, sharedThis = shared_from_this()](
                                                            const boost::system::error_code& error,
                                                            const std::shared_ptr<Schedulers::TimerDescriptor>&
                                                            desriptor)
                                                            {
                                                              return false;
                                                            });

      m_timerDisposable = GetScheduler()->RegisterTimer(timer);
    }

    void BuyTicketProcessCoordinator::publishRestartEvent()
    {
      DisposeTimer();
      m_selectedTicket = INVALID_TICKET;
      GetMessenger().PublishEvent(make_shared<Events::RestartEvent>());
    }

    void BuyTicketProcessCoordinator::OnPrintTicketResponseEvent(Events::PrintTicketResponseEvent& e)
    {
      publishRestartEvent();
    }

    void BuyTicketProcessCoordinator::ReleaseAllSubscriptions()
    { 
      DisposeTimer();
      m_subscriptionsDisposable->Dispose();
    }

    void BuyTicketProcessCoordinator::OnCanceledEvent(Events::CanceledEvent& e)
    {
      publishRestartEvent();
    }
  }
}
