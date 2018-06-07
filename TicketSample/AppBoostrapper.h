#pragma once
#include "RStein.Common/DisposableResource.h"
#include <fstream>

namespace EventProcessing {
  class Messenger;
}

namespace TicketApp {
  namespace Infrastructure {
    class Logger;
  }
}

namespace TicketApp {
  namespace ExternalPorts {
    class Printer;
  }
}

namespace TicketApp {
  namespace Ui {
    class Display;
  }
}

namespace TicketApp {
  namespace InternalProcessors {
    class PaymentProcessor;
    class BuyTicketProcessCoordinator;
  }
}

namespace Schedulers {
  class StrictStrandSchedulerDecorator;
  class ThreadPoolScheduler;
}

class AppBoostrapper
{

public:
  AppBoostrapper() = default;
  ~AppBoostrapper() = default;
  AppBoostrapper(const AppBoostrapper& other) = delete;
  AppBoostrapper(AppBoostrapper&& other) noexcept = delete;
  AppBoostrapper& operator=(const AppBoostrapper& other) = delete;
  AppBoostrapper& operator=(AppBoostrapper&& other) noexcept = delete;


  void Start();
  void Stop();

private:
  std::shared_ptr<Schedulers::ThreadPoolScheduler> m_threadPoolScheduler{};
  std::shared_ptr<Schedulers::StrictStrandSchedulerDecorator> m_coordinatorScheduler{};
  std::shared_ptr<Schedulers::StrictStrandSchedulerDecorator> m_messengerScheduler{};
  std::shared_ptr<TicketApp::InternalProcessors::BuyTicketProcessCoordinator> m_buyTicketProcessor{};
  std::shared_ptr<Schedulers::StrictStrandSchedulerDecorator> m_paymentProcessorScheduler{};
  std::shared_ptr<TicketApp::InternalProcessors::PaymentProcessor> m_paymentProcessor{};
  std::shared_ptr<Schedulers::StrictStrandSchedulerDecorator> m_displayScheduler{};
  std::shared_ptr<TicketApp::Ui::Display> m_display{};
  std::shared_ptr<Schedulers::StrictStrandSchedulerDecorator> m_printerScheduler{};
  std::shared_ptr<TicketApp::ExternalPorts::Printer> m_printer{};
  std::shared_ptr<Schedulers::StrictStrandSchedulerDecorator> m_loggerScheduler{};
  std::shared_ptr<TicketApp::Infrastructure::Logger> m_logger{};
  std::shared_ptr<EventProcessing::Messenger> m_messenger{};
  std::shared_ptr<RStein::Utils::DisposableResource> m_pseudoOwner{};
  RStein::Utils::DisposablePtr m_restartSubscription{};
  std::ofstream m_fileStream{};
  RStein::Utils::DisposablePtr m_payementReceivedSubscription{};
};
