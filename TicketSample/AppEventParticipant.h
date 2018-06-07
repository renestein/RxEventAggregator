#pragma once
#include <utility>
#include "Async/AsyncServiceBase.h"
#include "RStein.Common/CompositeDisposable.h"
namespace EventProcessing {
  class Messenger;
}

namespace TicketApp
{
  namespace Events {
    class QuitEvent;
  }

  class AppEventParticipant : public Async::AsyncServiceBase
  {
  public:
    using MyBase = Async::AsyncServiceBase;
    AppEventParticipant(const std::shared_ptr<::Schedulers::Scheduler>& scheduler,
      std::shared_ptr<EventProcessing::Messenger> messenger);
  protected:
    EventProcessing::Messenger& GetMessenger() const;
    void TryRunQuitLogic();
    virtual void OnQuitEvent(Events::QuitEvent& quitEvent);
    void OnStart() override;
    void OnStop() override;
    virtual void ReleaseAllSubscriptions() = 0;
  private:
    std::shared_ptr<EventProcessing::Messenger> m_messenger;
    RStein::Utils::CancelToken m_quitCancelToken;
    RStein::Utils::DisposablePtr m_quitEventSubscription;
  };
}
