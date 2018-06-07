#include "stdafx.h"
#include "Logger.h"
#include "../../RXEventAggregator/Lib/EventProcessing/Event.h"
#include "../../RXEventAggregator/Lib/EventProcessing/EventAggregator.h"
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace EventProcessing;
using namespace TicketApp::Events;

namespace TicketApp
{
  namespace Infrastructure
  {
    Logger::Logger(const std::shared_ptr<EventAggregator<Event>>& messenger, std::ostream& outputStream) :
      MyBase{messenger},
      _outputOstream{outputStream},
      _registration{}
    {
    }

    void Logger::OnStart()
    {
      MyBase::OnStart();
      _registration = GetMessenger().GetEventStream<Event>().subscribe([this](std::shared_ptr<Event> event)
      {
        auto eventTime = event->GetEventTime();
        _outputOstream << " Event data: "
          << event->ToString()
          << " Event creation date: "
          << ctime(&eventTime)
          << endl
          << endl;
      });
    };

    void Logger::ReleaseAllSubscriptions()
    {
      _registration.unsubscribe();
    }
  }
}
