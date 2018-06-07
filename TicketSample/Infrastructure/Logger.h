#pragma once
#include "TicketApp/AppEventParticipant.h"
#include <iostream>
namespace TicketApp
{
  namespace Infrastructure
  {
    class Logger : public AppEventParticipant
    {
    public:
      using MyBase = AppEventParticipant;
      Logger(const std::shared_ptr<::Schedulers::Scheduler>& scheduler,
             const std::shared_ptr<EventProcessing::Messenger>& messenger,
             std::ostream& outputStream); 
    protected:
      void OnStart() override;
      void ReleaseAllSubscriptions() override;
    private:
      std::ostream& m_outputOstream;
      RStein::Utils::DisposablePtr m_registration;
    };
  }
}