#include "stdafx.h"
#include "AppBoostrapper.h"
#include "../RXEventAggregator/Lib/EventProcessing/EventAggregator.h"
#include "InternalProcessors/BuyTicketProcessCoordinator.h"
#include "InternalProcessors/PaymentProcessor.h"
#include "UI/Display.h"
#include "ExternalPorts/Printer.h"
#include "Infrastructure/Logger.h"
#include "Events/RestartEvent.h"
#include "Events/PaymentReceivedEvent.h"
#include <memory>
#include <thread>

using namespace std;
using namespace Schedulers;
using namespace TicketApp::InternalProcessors;
using namespace TicketApp::Infrastructure;
using namespace TicketApp::ExternalPorts;
using namespace TicketApp::Ui;
using namespace TicketApp::Events;
using namespace EventProcessing;

void AppBoostrapper::Start()
{
  _messenger = make_shared<EventAggregator<Event>>();

  _fileStream.open("c:\\_Archiv\\ME_log.txt", ofstream::out | ofstream::trunc);
  _logger = make_shared<Logger>(_messenger, _fileStream);
  _logger->Start();

  _paymentProcessor = make_shared<PaymentProcessor>(_messenger);
  _paymentProcessor->Start();

  _printer = make_shared<Printer>(_messenger);
  _printer->Start();

  _display = make_shared<Display>(_messenger);
  _display->Start();

  _restartSubscription = _messenger->GetEventStream<RestartEvent>().subscribe([this](shared_ptr<RestartEvent> event)
  {
    const int STANDARD_TICKET = 1;
    _display->SelectTicketButtonPressed(STANDARD_TICKET);
    _display->PaymentButtonPressed();
  });

  _paymentReceivedSubscription = _messenger->GetEventStream<PaymentReceivedEvent>().
                                              subscribe([this](shared_ptr<PaymentReceivedEvent> event)
                                              {
                                                if (event->GetCurrentAmount() != 0)
                                                {
                                                  return;
                                                }

                                                static bool payFullPrice = true;
                                                const int STANDARD_TICKET_PRICE = 20;

                                                auto payPrice = payFullPrice
                                                                  ? STANDARD_TICKET_PRICE
                                                                  : STANDARD_TICKET_PRICE - 1;

                                                payFullPrice = !payFullPrice;
                                                for (auto i = 0; i < payPrice; i++)
                                                {
                                                  _paymentProcessor->AddCoin(1);
                                                }
                                              });

  ///Simulate inputs (dirty and fragile code)

  _buyTicketProcessor = make_shared<BuyTicketProcessCoordinator>(_messenger);
  _buyTicketProcessor->Start();
}

void AppBoostrapper::Stop()
{
  //UI driver
  _restartSubscription.unsubscribe();
  _paymentReceivedSubscription.unsubscribe();
  //UI driver


  _buyTicketProcessor->Stop();
  _buyTicketProcessor.reset();


  _display->Stop();
  _display.reset();

  _printer->Stop();
  _printer.reset();

  _paymentProcessor->Stop();
  _paymentProcessor.reset();

  _logger->Stop();
  _logger.reset();

  _fileStream.flush();
  _fileStream.close();
  _messenger.reset();
}
