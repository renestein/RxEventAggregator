#include "stdafx.h"
#include "AppBoostrapper.h"
#include "Schedulers/StrictStrandSchedulerDecorator.h"
#include <memory>
#include "InternalProcessors/BuyTicketProcessCoordinator.h"
#include "InternalProcessors/PaymentProcessor.h"
#include "UI/Display.h"
#include "ExternalPorts/Printer.h"
#include "Infrastructure/Logger.h"
#include "EventProcessing/Messenger.h"
#include "RStein.Common/DisposableResource.h"
#include "Events/RestartEvent.h"
#include "Events/PaymentReceivedEvent.h"
#include <thread>

using namespace std;
using namespace Schedulers;
using namespace TicketApp::InternalProcessors;
using namespace TicketApp::Infrastructure;
using namespace TicketApp::ExternalPorts;
using namespace TicketApp::Ui;
using namespace TicketApp::Events;
using namespace EventProcessing;
using namespace RStein::Utils;

void AppBoostrapper::Start()
{
  m_threadPoolScheduler = std::make_shared<ThreadPoolScheduler>(std::thread::hardware_concurrency());
  m_threadPoolScheduler->Start();

  m_messengerScheduler = make_shared<StrictStrandSchedulerDecorator>(m_threadPoolScheduler);
  m_messengerScheduler->Start();
  m_messenger = make_shared<Messenger>(m_messengerScheduler);
  m_messenger->AsyncStart().wait();

  m_loggerScheduler = make_shared<StrictStrandSchedulerDecorator>(m_threadPoolScheduler);
  m_loggerScheduler->Start();
  m_fileStream.open("c:\\_Archiv\\ME_log.txt", ofstream::out | ofstream::trunc);
  m_logger = make_shared<Logger>(m_loggerScheduler, m_messenger, m_fileStream);
  m_logger->AsyncStart().wait();

  m_paymentProcessorScheduler = make_shared<StrictStrandSchedulerDecorator>(m_threadPoolScheduler);
  m_paymentProcessorScheduler->Start();
  m_paymentProcessor = make_shared<PaymentProcessor>(m_paymentProcessorScheduler, m_messenger);
  m_paymentProcessor->AsyncStart().wait();

  m_printerScheduler = make_shared<StrictStrandSchedulerDecorator>(m_threadPoolScheduler);
  m_printerScheduler->Start();
  m_printer = make_shared<Printer>(m_printerScheduler, m_messenger);
  m_printer->AsyncStart().wait();

  m_displayScheduler = make_shared<StrictStrandSchedulerDecorator>(m_threadPoolScheduler);
  m_displayScheduler->Start();
  m_display = make_shared<Display>(m_displayScheduler, m_messenger);
  m_display->AsyncStart().wait();

  //Simulate inputs (dirty and fragile code)
  m_pseudoOwner = make_shared<DisposableResource>([]
  {
  });

  m_restartSubscription = m_messenger->RegisterEventHandler<DisposableResource, RestartEvent>(m_pseudoOwner, [this](shared_ptr<RestartEvent> event)
  {
    const int STANDARD_TICKET = 1;
    m_display->SelectTicketButtonPressed(STANDARD_TICKET);
    m_display->PaymentButtonPressed();
  });

  m_payementReceivedSubscription = m_messenger->RegisterEventHandler<DisposableResource, PaymentReceivedEvent>(m_pseudoOwner,
    [this](shared_ptr<PaymentReceivedEvent> event)
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
      m_paymentProcessor->AddCoin(1);
    }
  });

  ///Simulate inputs (dirty and fragile code)

  m_coordinatorScheduler = make_shared<StrictStrandSchedulerDecorator>(m_threadPoolScheduler);
  m_coordinatorScheduler->Start();
  m_buyTicketProcessor = make_shared<BuyTicketProcessCoordinator>(m_coordinatorScheduler, m_messenger);
  m_buyTicketProcessor->AsyncStart().wait();
  //
}

void AppBoostrapper::Stop()
{
  //UI driver
  m_restartSubscription->Dispose();
  //m_payementReceivedSubscription->Dispose();
  //UI driver



  m_buyTicketProcessor->AsyncStop().wait();
  m_coordinatorScheduler->Stop();
  m_buyTicketProcessor.reset();
  m_coordinatorScheduler.reset();



  m_display->AsyncStop().wait();
  m_displayScheduler->Stop();
  m_display.reset();
  m_displayScheduler.reset();

  m_printer->AsyncStop().wait();
  m_printerScheduler->Stop();
  m_printer.reset();
  m_printerScheduler.reset();

  m_paymentProcessor->AsyncStop().wait();
  m_paymentProcessorScheduler->Stop();
  m_paymentProcessor.reset();
  m_paymentProcessorScheduler.reset();

  m_logger->AsyncStop().wait();
  m_loggerScheduler->Stop();
  m_logger.reset();
  m_loggerScheduler.reset();

  m_fileStream.flush();
  m_fileStream.close();
  m_messenger->AsyncStop().wait();
  m_messengerScheduler->Stop();
  m_messenger.reset();
  m_messengerScheduler.reset();



  m_threadPoolScheduler->Stop();
  m_threadPoolScheduler.reset();
}
