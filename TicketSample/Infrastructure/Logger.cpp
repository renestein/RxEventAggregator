#include "stdafx.h"
#include "Logger.h"
#include "EventProcessing/Messenger.h"
#include "TicketApp/Events/EventTypes.h"
#include <chrono>
#include "Misc/DateTime.h"

using namespace std;
using namespace std::chrono;
using namespace EventProcessing;
using namespace TicketApp::Events;

namespace TicketApp
{
  namespace Infrastructure
  {
    Logger::Logger(const std::shared_ptr<::Schedulers::Scheduler>& scheduler,
                   const std::shared_ptr<EventProcessing::Messenger>& messenger, std::ostream& outputStream) :
      MyBase{scheduler, messenger},
      m_outputOstream{outputStream},
      m_registration{}
    {
    }

    void Logger::OnStart()
    {
      MyBase::OnStart();
      m_registration = GetMessenger().RegisterEventHandler(shared_from_this(), [this](std::shared_ptr<Event> event)
      {
        ScheduleFutureOperation([event, this]()
        {
          auto currentTime = Misc::DateTime::GetCurrentDateTime();
          auto eventTime = event->GetEventTime();
          m_outputOstream << ctime(&currentTime)
            << " Event data: "
            << event->ToString()
            << " Event creation date: "
            << ctime(&eventTime)
            << endl
            << endl;
        }, "Logger handler");
      }, APP_EVENT_ALL);
    }

    void Logger::ReleaseAllSubscriptions()
    {
      m_registration->Dispose();
      m_registration.reset();
    }
  }
}