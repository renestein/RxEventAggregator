﻿#include "stdafx.h"
#include "Display.h"
#include "../../RXEventAggregator/Lib/EventProcessing/EventAggregator.h"
#include "../Events/PaymentReceivedEvent.h"
#include "../Events/RestartEvent.h"
#include "../Events/TicketSelectedEvent.h"
#include "../Events/PaymentRequestEvent.h"
#include "../Events/PrintTicketRequestEvent.h"
#include "../Events/PrintTicketResponseEvent.h"
#include "../Events/CanceledEvent.h"
#include "../Events/EventTypes.h"
#include <memory>
#include <iostream>
using namespace std;
using namespace TicketApp::Events;

namespace TicketApp
{
  namespace Ui
  {
    Display::Display(
      const std::shared_ptr<EventProcessing::EventAggregator<EventProcessing::Event>>& messenger) : MyBase(messenger)
    {
    }

    void Display::CancelButtonPressed()
    {
      GetMessenger().PublishEvent(std::make_shared<CanceledEvent>());
    }

    void Display::SelectTicketButtonPressed(TicketType selectedTicket)
    {
      GetMessenger().PublishEvent(std::make_shared<TicketSelectedEvent>(selectedTicket));
    }

    void Display::PaymentButtonPressed()
    {
      GetMessenger().PublishEvent(std::make_shared<PaymentRequestEvent>());
    }

    void Display::Start()
    {
      cout << "Inicializace. Prosim cekejte..." << endl;
      MyBase::Start();
      _compositeSubscription = GetMessenger()
                               .GetEventStream()
                               .filter([](auto event)
                               {
                                 auto eventType = event->GetEventType();
                                 return eventType == APP_EVENT_PAYMENT_RECEIVED ||
                                        eventType == APP_EVENT_RESTART ||
                                        eventType == APP_EVENT_PRINT_TICKET_REQUEST ||
                                        eventType == APP_EVENT_PRINT_TICKET_RESPONSE;
                               }).subscribe([this](auto event)
                               {
                                 switch (event->GetEventType())
                                 {
                                 case APP_EVENT_PAYMENT_RECEIVED:
                                   {
                                     OnPaymentReceivedEvent(*dynamic_pointer_cast<PaymentReceivedEvent>(event));
                                     break;
                                   }

                                 case APP_EVENT_RESTART:
                                   {
                                     OnRestartEvent(*dynamic_pointer_cast<RestartEvent>(event));
                                     break;
                                   }
                                 case APP_EVENT_PRINT_TICKET_REQUEST:
                                   {
                                     OnPrintTicketRequestEvent(*dynamic_pointer_cast<PrintTicketRequestEvent>(event));
                                     break;
                                   }

                                 case APP_EVENT_PRINT_TICKET_RESPONSE:
                                   {
                                     OnPrintTicketResponseEvent(*dynamic_pointer_cast<PrintTicketResponseEvent>(event));
                                     break;
                                   }

                                 default:
                                   break;
                                 }
                               });
    }

    void Display::OnPaymentReceivedEvent(const PaymentReceivedEvent& event)
    {
      if (event.GetCurrentAmount() == event.GetRequiredAmount())
      {
        _cancelButtonEnabled = false;
      }

      cout << "Zaplaceno: "
        << event.GetCurrentAmount()
        << " Celkova castka: "
        << event.GetRequiredAmount()
        << endl;
    }

    void Display::OnRestartEvent(const RestartEvent& e)
    {
      std::cout << std::this_thread::get_id();
      _selectTicketButtonEnabled = true;
      _paymentButtonEnabled = _cancelButtonEnabled = false;
      cout << "Vyberte typ listku." << endl;
    }

    void Display::OnPrintTicketRequestEvent(const PrintTicketRequestEvent& e)
    {
      cout << "Tisknu listek..." << endl;
    }

    void Display::OnPrintTicketResponseEvent(const PrintTicketResponseEvent& e)
    {
      std::cout << std::this_thread::get_id();
      cout << "Dekujeme za zakoupeni listku." << endl;
    }

    void Display::ReleaseAllSubscriptions()
    {
      _compositeSubscription.unsubscribe();
    }
  }
}
