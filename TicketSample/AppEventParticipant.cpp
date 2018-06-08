#include "stdafx.h"
#include "AppEventParticipant.h"
#include "../RXEventAggregator/Lib/EventProcessing/EventAggregator.h"
#include "../RXEventAggregator/Lib/EventProcessing/Event.h"
#include "Events/QuitEvent.h"
#include <memory>

using namespace std;
using namespace EventProcessing;

namespace TicketApp
{
  AppEventParticipant::
  AppEventParticipant(std::shared_ptr<EventAggregator<Event>> messenger) :
    _messenger(std::move(messenger))
  {
    if (!_messenger)
    {
      std::invalid_argument messengerInvalidException("messenger");
      throw messengerInvalidException;
    }
  }

  EventAggregator<Event>& AppEventParticipant::GetMessenger() const
  {
    return *_messenger;
  }

  void AppEventParticipant::TryRunQuitLogic()
  {
    _quitEventSubscription.unsubscribe();
    ReleaseAllSubscriptions();
  }

  void AppEventParticipant::OnQuitEvent(Events::QuitEvent& quitEvent)
  {
    TryRunQuitLogic();
  }

  void AppEventParticipant::Start()
  {
    _quitEventSubscription = GetMessenger().GetEventStream<Events::QuitEvent>().subscribe(
                                                                                          [this](
                                                                                          shared_ptr<Events::QuitEvent>
                                                                                          quitEvent)
                                                                                          {
                                                                                            OnQuitEvent(*quitEvent);
                                                                                          });
  }

  void AppEventParticipant::Stop()
  {
    TryRunQuitLogic();
  }

  void AppEventParticipant::ReleaseAllSubscriptions()
  {
  }
}
