#include "stdafx.h"
#include "PaymentProcessor.h"
#include "../Events/ErrorEvent.h"
#include "../../RXEventAggregator/Lib/EventProcessing/EventAggregator.h"
#include "../Events/TicketSelectedEvent.h"
#include "../Events/PaymentReceivedEvent.h"
#include "../Events/RestartEvent.h"
#include "../Events/CanceledEvent.h"
#include "../Events/PaymentRequestEvent.h"
#include <memory>

using namespace std;

namespace TicketApp
{
  namespace InternalProcessors
  {
    void PaymentProcessor::AddCoin(int amount)
    {
      //check amount;
      if (_currentAmount == INVALID_AMOUNT)
      {
        auto errorEvent = make_shared<Events::ErrorEvent>("Error in PaymentProcessor - unexpected amount");
        GetMessenger().PublishEvent(errorEvent);
        return;
      }

      _currentAmount += amount;
      //Nepočítáme s větší částkou a "rozměněním" peněz

      publishPaymentReceived();
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
      if (_currentAmount != INVALID_AMOUNT)
      {
        publishUnexpectedEvent();
        return;
      }

      const int STANDARD_TICKET_PRICE = 20;
      const int CHILD_TICKET_PRICE = 10;
      const int STANDARD_TICKET_ID = 1;
      const int CHILD_TICKET_ID = 2;

      _currentAmount = 0;
      _requiredAmount = event.GetTicketType() == STANDARD_TICKET_ID
                          ? STANDARD_TICKET_PRICE
                          : CHILD_TICKET_PRICE;
    }

    void PaymentProcessor::publishPaymentReceived()
    {
      auto paymentReceivedEvent = make_shared<Events::PaymentReceivedEvent>(_currentAmount, _requiredAmount);
      GetMessenger().PublishEvent(paymentReceivedEvent);
    }

    void PaymentProcessor::OnPaymentRequestEvent(const Events::PaymentRequestEvent& event)
    {
      if (_currentAmount != 0)
      {
        publishUnexpectedEvent();
      }


      publishPaymentReceived();
    }

    void PaymentProcessor::clearAmounts()
    {
      _currentAmount = INVALID_AMOUNT;
      _requiredAmount = INVALID_AMOUNT;
    }

    void PaymentProcessor::returnMoney()
    {
      if (_currentAmount > 0)
      {
        cout << "Vracim penize: " << _currentAmount << endl;
        clearAmounts();
      }
    }

    void PaymentProcessor::OnCanceledEvent(const Events::CanceledEvent& event)
    {
      returnMoney();
    }

    void PaymentProcessor::Start()
    {
      MyBase::Start();
      _compositeSubscription = GetMessenger().GetEventStream<Events::RestartEvent>().subscribe(
                                                                                               [this](
                                                                                               shared_ptr<Events::
                                                                                                 RestartEvent>
                                                                                               restartEvent)
                                                                                               {
                                                                                                 OnRestartEvent(*restartEvent);
                                                                                               });

      auto canceledEventSubscription = GetMessenger()
                                       .GetEventStream<Events::CanceledEvent>().subscribe(_compositeSubscription,
                                                                                          [this](
                                                                                          shared_ptr<Events::
                                                                                            CanceledEvent>
                                                                                          canceledEvent)
                                                                                          {
                                                                                            OnCanceledEvent(*canceledEvent);
                                                                                          });


      auto ticketSelectedEventSubscription = GetMessenger()
                                             .GetEventStream<Events::TicketSelectedEvent>().
                                             subscribe(_compositeSubscription,
                                                       [this](
                                                       shared_ptr<Events::TicketSelectedEvent> ticketSelectedEvent)
                                                       {
                                                         OnTicketSelectedEvent(*ticketSelectedEvent);
                                                       });

      auto paymentRequestEventSubscription = GetMessenger()
                                             .GetEventStream<Events::PaymentRequestEvent>().
                                             subscribe(_compositeSubscription,
                                                       [this](
                                                       shared_ptr<Events::PaymentRequestEvent> paymentRequestEvent)
                                                       {
                                                         OnPaymentRequestEvent(*paymentRequestEvent);
                                                       });
    }

    void PaymentProcessor::ReleaseAllSubscriptions()
    {
      _compositeSubscription.unsubscribe();
    }
  }
}
