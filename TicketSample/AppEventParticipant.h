#pragma once
#include <rxcpp/rx-subscription.hpp>

namespace EventProcessing
{
  class Event;
  template <typename TBE>
  class EventAggregator;
}

namespace TicketApp
{
  namespace Events
  {
    class QuitEvent;
  }

  class AppEventParticipant
  {
  public:
    AppEventParticipant(std::shared_ptr<EventProcessing::EventAggregator<EventProcessing::Event>> messenger);
    virtual void Start();
    virtual void Stop();
  protected:
    EventProcessing::EventAggregator<EventProcessing::Event>& GetMessenger() const;
    void TryRunQuitLogic();
    virtual void OnQuitEvent(Events::QuitEvent& quitEvent);
    virtual void ReleaseAllSubscriptions() = 0;
  private:
    std::shared_ptr<EventProcessing::EventAggregator<EventProcessing::Event>> _messenger;
    rxcpp::composite_subscription _quitEventSubscription;
  };
}
