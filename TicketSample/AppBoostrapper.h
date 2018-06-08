#pragma once
#include <fstream>
#include "../RXEventAggregator/Lib/EventProcessing/Event.h"
#include "../RXEventAggregator/Lib/EventProcessing/EventAggregator.h"

namespace EventProcessing {
  template<typename TBE>
  class Messenger;
  class Event;
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
 
  std::shared_ptr<TicketApp::InternalProcessors::BuyTicketProcessCoordinator> _buyTicketProcessor{};
  std::shared_ptr<TicketApp::InternalProcessors::PaymentProcessor> _paymentProcessor{}; 
  std::shared_ptr<TicketApp::Ui::Display> _display{};
  std::shared_ptr<TicketApp::ExternalPorts::Printer> _printer{};
  std::shared_ptr<TicketApp::Infrastructure::Logger> _logger{};
  std::shared_ptr<EventProcessing::EventAggregator<EventProcessing::Event>> _messenger{};
  std::ofstream _fileStream{};
  rxcpp::composite_subscription _restartSubscription;
  rxcpp::composite_subscription _paymentReceivedSubscription;
};
