#pragma once
#include "../AppEventParticipant.h"

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

      Printer(const std::shared_ptr<EventProcessing::EventAggregator<EventProcessing::Event>>& messenger)
        : MyBase(messenger)
      {
      }

      void Start() override;
    protected:
      void OnPrintRequestEvent(const Events::PrintTicketRequestEvent& event);

      void ReleaseAllSubscriptions() override;
      rxcpp::composite_subscription m_printRequestSubscription;
    };
  }
}
