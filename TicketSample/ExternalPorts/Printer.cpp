#include "stdafx.h"
#include "Printer.h"
#include "TicketApp/Events/PrintTicketRequestEvent.h"
#include "TicketApp/Events/PrintTicketResponseEvent.h"
#include "EventProcessing/Messenger.h"
#include <memory>
using namespace std;

namespace TicketApp
{
  namespace ExternalPorts
  {
    void Printer::OnStart()
    {
      MyBase::OnStart();
      m_printRequestSubscription = GetMessenger().RegisterEventHandler<
        AsyncServiceBase, Events::PrintTicketRequestEvent>(shared_from_this(),
                                                           [this](
                                                           shared_ptr<Events::PrintTicketRequestEvent>
                                                           printRequestEvent)
                                                           {
                                                             ScheduleFutureOperation([printRequestEvent, this]
                                                             {
                                                               OnPrintRequestEvent(*printRequestEvent);
                                                             }, "PaymentProcessor.OnPrintRequestEvent");
                                                           });
    }


    void Printer::OnPrintRequestEvent(const Events::PrintTicketRequestEvent& event)
    {
      //print
      auto printTicketResponseEvent = make_shared<Events::PrintTicketResponseEvent>();
      /*ScheduleFutureOperation([printTicketResponseEvent, this]
      {
      */
      GetMessenger().PublishEvent(printTicketResponseEvent);
      /*}, __FUNCTION__);*/
    }

    void Printer::ReleaseAllSubscriptions()
    {
      m_printRequestSubscription->Dispose();
    }
  }
}
