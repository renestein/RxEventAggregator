#include "stdafx.h"
#include "Display.h"
#include "EventProcessing/Messenger.h"
#include "TicketApp/Events/PaymentReceivedEvent.h"
#include "TicketApp/Events/RestartEvent.h"
#include "TicketApp/Events/TicketSelectedEvent.h"
#include "TicketApp/Events/PaymentRequestEvent.h"
#include "TicketApp/Events/PrintTicketRequestEvent.h"
#include "TicketApp/Events/PrintTicketResponseEvent.h"
#include "TicketApp/Events/CanceledEvent.h"
#include <memory>
#include <iostream>
using namespace std;
using namespace TicketApp::Events;

namespace TicketApp
{
  namespace Ui
  {
    Display::Display(const std::shared_ptr<Schedulers::Scheduler>& scheduler,
                     const std::shared_ptr<EventProcessing::Messenger>& messenger) : MyBase(scheduler, messenger)
    {
    }

    void Display::CancelButtonPressed()
    {
      ScheduleFutureOperation([this]
      {
        if (m_cancelButtonEnabled)
        {
          m_cancelButtonEnabled = false;
          GetMessenger().PublishEvent(std::make_shared<CanceledEvent>());
        }
      }, __FUNCTION__);
    }

    void Display::SelectTicketButtonPressed(TicketType selectedTicket)
    {
      ScheduleFutureOperation([this, selectedTicket]
      {
        if (m_selectTicketButtonEnabled)
        {
          m_selectTicketButtonEnabled = false;
   
          m_cancelButtonEnabled = true;
          m_paymentButtonEnabled = true;
          GetMessenger().PublishEvent(std::make_shared<TicketSelectedEvent>(selectedTicket));
        }
      }, __FUNCTION__);
    }

    void Display::PaymentButtonPressed()
    {
      ScheduleFutureOperation([this]
      {
        if (m_paymentButtonEnabled)
        {
          m_paymentButtonEnabled = false;
          GetMessenger().PublishEvent(std::make_shared<PaymentRequestEvent>());
        }
      }, __FUNCTION__);
    }

    void Display::OnStart()
    {
      cout << "Inicializace. Prosim cekejte..." << endl;
      MyBase::OnStart();
      auto paymentEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, PaymentReceivedEvent>(shared_from_this(),
                                                [this](shared_ptr<PaymentReceivedEvent> paymentReceivedEvent)
                                                {
                                                  ScheduleFutureOperation([paymentReceivedEvent, this]
                                                  {
                                                    OnPaymentReceivedEvent(*paymentReceivedEvent);
                                                  }, "Display.OnPaymentReceived");
                                                });

      auto restartEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, RestartEvent>(shared_from_this(),
                                        [this](shared_ptr<RestartEvent> restartEvent)
                                        {
                                          ScheduleFutureOperation([restartEvent, this]
                                          {
                                            OnRestartEvent(*restartEvent);
                                          }, "Display.OnRestartReceived");
                                        });

       auto printRequestEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, PrintTicketRequestEvent>(shared_from_this(),
                                        [this](shared_ptr<PrintTicketRequestEvent> printTicketRequestEvent)
                                        {
                                          ScheduleFutureOperation([printTicketRequestEvent, this]
                                          {
                                            OnPrintTicketRequestEvent(*printTicketRequestEvent);
                                          }, "Display.OnPrintTicketRequestEvent");
                                        });

      
       auto printResponseEventSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, PrintTicketResponseEvent>(shared_from_this(),
                                        [this](shared_ptr<PrintTicketResponseEvent> printTicketResponseEvent)
                                        {
                                          ScheduleFutureOperation([printTicketResponseEvent, this]
                                          {
                                            OnPrintTicketResponseEvent(*printTicketResponseEvent);
                                          }, "Display.OnPrintTicketResponseEvent");
                                        });

      m_compositeDisposable = make_shared<RStein::Utils::CompositeDisposableResource>();
      m_compositeDisposable->registerDisposable(paymentEventSubscription);
      m_compositeDisposable->registerDisposable(restartEventSubscription);
      m_compositeDisposable->registerDisposable(printRequestEventSubscription);
      m_compositeDisposable->registerDisposable(printResponseEventSubscription);
    }

    void Display::OnPaymentReceivedEvent(const PaymentReceivedEvent& event)
    {
      if (event.GetCurrentAmount() == event.GetRequiredAmount())
      {
        m_cancelButtonEnabled = false;
      }

      cout << "Zaplaceno: "
        << event.GetCurrentAmount()
        << " Celkova castka: "
        << event.GetRequiredAmount()
        << endl;
    }

    void Display::OnRestartEvent(const RestartEvent& e)
    {
      m_selectTicketButtonEnabled = true;
      m_paymentButtonEnabled = m_cancelButtonEnabled = false;
      cout << "Vyberte typ listku." << endl;
    }

    void Display::OnPrintTicketRequestEvent(const Events::PrintTicketRequestEvent& e)
    {
      cout << "Tisknu listek..." << endl;
    }

    void Display::OnPrintTicketResponseEvent(const Events::PrintTicketResponseEvent& e)
    {
      cout << "Dekujeme za zakoupeni listku." << endl;
    }

    void Display::ReleaseAllSubscriptions()
    {
       m_compositeDisposable->Dispose();
    }
  }
}
