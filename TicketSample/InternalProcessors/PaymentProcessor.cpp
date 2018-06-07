#include "stdafx.h"
#include "PaymentProcessor.h"
#include "EventProcessing/Messenger.h"
#include "TicketApp/Events/TicketSelectedEvent.h"
#include "TicketApp/Events/RestartEvent.h"
#include "TicketApp/Events/PaymentReceivedEvent.h"
#include "TicketApp/Events/PaymentRequestEvent.h"
#include "TicketApp/Events/ErrorEvent.h"
#include "TicketApp/Events/CanceledEvent.h"
#include <memory>

using namespace std;

namespace TicketApp
{
  namespace InternalProcessors
  {
    void PaymentProcessor::AddCoin(int amount)
    {
      ScheduleFutureOperation([this, amount]
      {
        //check amount;
        if (m_currentAmount == INVALID_AMOUNT)
        {
          auto errorEvent = make_shared<Events::ErrorEvent>("Error in PaymentProcessor - unexpected amount");
          GetMessenger().PublishEvent(errorEvent);
          return;
        }

        m_currentAmount += amount;
        //Nepočítáme s větší částkou a "rozměněním" peněz

        publishPaymentReceived();
      }, __FUNCTION__);
    }

    void PaymentProcessor::OnRestartEvent(const Events::RestartEvent& event)
    {
      clearAmounts();
    }

    void PaymentProcessor::publishUnexpectedEvent() const
    {
      auto errorEvent = make_shared<Events::ErrorEvent>("Error in PaymentProcessor - unexpected event");
      GetMessenger().PublishEvent(errorEvent);
    }

    void PaymentProcessor::OnTicketSelectedEvent(const Events::TicketSelectedEvent& event)
    {
      if (m_currentAmount != INVALID_AMOUNT)
      {
        publishUnexpectedEvent();
        return;
      }

      const int STANDARD_TICKET_PRICE = 20;
      const int CHILD_TICKET_PRICE = 10;
      const int STANDARD_TICKET_ID = 1;
      const int CHILD_TICKET_ID = 2;

      m_currentAmount = 0;
      m_requiredAmount = event.GetTicketType() == STANDARD_TICKET_ID
                           ? STANDARD_TICKET_PRICE
                           : CHILD_TICKET_PRICE;
    }

    void PaymentProcessor::publishPaymentReceived()
    {
      auto paymentReceivedEvent = make_shared<Events::PaymentReceivedEvent>(m_currentAmount, m_requiredAmount);
      GetMessenger().PublishEvent(paymentReceivedEvent);
    }

    void PaymentProcessor::OnPaymentRequestEvent(const Events::PaymentRequestEvent& event)
    {
      if (m_currentAmount != 0)
      {
        publishUnexpectedEvent();
      }


      publishPaymentReceived();
    }

    void PaymentProcessor::clearAmounts()
    {
      m_currentAmount = INVALID_AMOUNT;
      m_requiredAmount = INVALID_AMOUNT;
    }

    void PaymentProcessor::returnMoney()
    {
      if (m_currentAmount > 0)
      {
        cout << "Vracim penize: " << m_currentAmount << endl;
        clearAmounts();
      }
    }

    void PaymentProcessor::OnCanceledEvent(const Events::CanceledEvent& event)
    {
      returnMoney();
    }

    void PaymentProcessor::OnStart()
    {
      MyBase::OnStart();
      auto restartEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, Events::RestartEvent>(shared_from_this(),
                                                [this](shared_ptr<Events::RestartEvent> restartEvent)
                                                {
                                                  ScheduleFutureOperation([restartEvent, this]
                                                  {
                                                    OnRestartEvent(*restartEvent);
                                                  }, "PaymentProcessor.OnRestartEvent");
                                                });
      
      auto canceledEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, Events::CanceledEvent>(shared_from_this(),
                                                [this](shared_ptr<Events::CanceledEvent> canceledEvent)
                                                {
                                                  ScheduleFutureOperation([canceledEvent, this]
                                                  {
                                                    OnCanceledEvent(*canceledEvent);
                                                  }, "PaymentProcessor.OnCanceledEvent");
                                                });


      auto ticketSelectedEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, Events::TicketSelectedEvent>(shared_from_this(),
                                                       [this](
                                                       shared_ptr<Events::TicketSelectedEvent> ticketSelectedEvent)
                                                       {
                                                         ScheduleFutureOperation([ticketSelectedEvent, this]
                                                         {
                                                           OnTicketSelectedEvent(*ticketSelectedEvent);
                                                         }, "PaymentProcessor.OnTicketSelectedEvent");
                                                       });

      auto paymentRequestEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, Events::PaymentRequestEvent>(shared_from_this(),
                                                       [this](
                                                       shared_ptr<Events::PaymentRequestEvent> paymentRequestEvent)
                                                       {
                                                         ScheduleFutureOperation([paymentRequestEvent, this]
                                                         {
                                                           OnPaymentRequestEvent(*paymentRequestEvent);
                                                         }, "PaymentProcessor.OnPaymentRequestEvent");
                                                       });

      m_subscriptions = make_shared<RStein::Utils::CompositeDisposableResource>();
      m_subscriptions->registerDisposable(ticketSelectedEventSubscription);
      m_subscriptions->registerDisposable(paymentRequestEventSubscription);
      m_subscriptions->registerDisposable(restartEventSubscription);
    }

    void PaymentProcessor::ReleaseAllSubscriptions()
    {
       m_subscriptions->Dispose();
    }
  }
}
