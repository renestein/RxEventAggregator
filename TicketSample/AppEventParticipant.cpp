#include "stdafx.h"
#include "AppEventParticipant.h"
#include "EventProcessing/Messenger.h"
#include "Events/QuitEvent.h"
#include <memory>

using namespace std;

namespace TicketApp
{
  AppEventParticipant::AppEventParticipant(const std::shared_ptr<::Schedulers::Scheduler>& scheduler,
                                           std::shared_ptr<EventProcessing::Messenger> messenger) : MyBase(scheduler),
                                                                                                    m_messenger(std::
                                                                                                                move(messenger)),
                                                                                                    m_quitCancelToken{}
  {
    if (!m_messenger)
    {
      std::invalid_argument messengerInvalidException("messenger");
      throw messengerInvalidException;
    }
  }

  EventProcessing::Messenger& AppEventParticipant::GetMessenger() const
  {
    return *m_messenger;
  }

  void AppEventParticipant::TryRunQuitLogic()
  {
    if (!m_quitCancelToken.Cancel())
    {
      return;
    }

    m_quitEventSubscription->Dispose();
    ReleaseAllSubscriptions();
  }

  void AppEventParticipant::OnQuitEvent(Events::QuitEvent& quitEvent)
  {
    TryRunQuitLogic();
  }

  void AppEventParticipant::OnStart()
  {
    m_quitEventSubscription = GetMessenger().RegisterEventHandler<
      AsyncServiceBase, Events::QuitEvent>(shared_from_this(),
                                           [this](shared_ptr<Events::QuitEvent> quitEvent)
                                           {
                                             ScheduleFutureOperation([quitEvent, this]
                                             {
                                               OnQuitEvent(*quitEvent);
                                             }, "AppEventParticipant.OnQuitEvent");
                                           });
  }

  void AppEventParticipant::OnStop()
  {
    TryRunQuitLogic();
  }

  void AppEventParticipant::ReleaseAllSubscriptions()
  {
  }
}
