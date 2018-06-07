#pragma once
#include "TicketApp/AppEventParticipant.h"
#include "RStein.Common/Disposable.h"

namespace TicketApp
{
}

namespace TicketApp
{
  namespace Events
  {
    class PrintTicketRequestEvent;
  }

  namespace ExternalPorts
  {
    class Printer : public AppEventParticipant
    {
    public:
      using MyBase = AppEventParticipant;

      Printer(const std::shared_ptr<::Schedulers::Scheduler>& scheduler,
              const std::shared_ptr<EventProcessing::Messenger>& messenger)
        : MyBase(scheduler, messenger)
      {
      }

    protected:
      void OnStart() override;
      void OnPrintRequestEvent(const Events::PrintTicketRequestEvent& event);

      void ReleaseAllSubscriptions() override;
      RStein::Utils::DisposablePtr m_printRequestSubscription;
    };
  }
}
