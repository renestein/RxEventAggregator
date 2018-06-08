#include "stdafx.h"
#include "Printer.h"
#include "../../RXEventAggregator/Lib/EventProcessing/EventAggregator.h"
#include "../Events/PrintTicketResponseEvent.h"
#include "../Events/PrintTicketRequestEvent.h"
#include <memory>
using namespace std;

namespace TicketApp
{
  namespace ExternalPorts
  {
    void Printer::Start()
    {
      MyBase::Start();
      m_printRequestSubscription = GetMessenger().GetEventStream<Events::PrintTicketRequestEvent>().subscribe(
                                                           [this](
                                                           shared_ptr<Events::PrintTicketRequestEvent>
                                                           printRequestEvent)
                                                           {
                                                               OnPrintRequestEvent(*printRequestEvent);                                                           
                                                           });
    }


    void Printer::OnPrintRequestEvent(const Events::PrintTicketRequestEvent& event)
    {

      auto printTicketResponseEvent = make_shared<Events::PrintTicketResponseEvent>();
      GetMessenger().PublishEvent(printTicketResponseEvent);
    }

    void Printer::ReleaseAllSubscriptions()
    {
      m_printRequestSubscription.unsubscribe();
    }
  }
}
