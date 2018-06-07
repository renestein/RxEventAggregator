#pragma once
#include "../AppEventParticipant.h"
#include <iostream>

namespace TicketApp
{
  namespace Infrastructure
  {
    class Logger : public AppEventParticipant
    {
    public:
      using MyBase = AppEventParticipant;
      Logger(const std::shared_ptr<EventProcessing::EventAggregator<EventProcessing::Event>>& messenger,
             std::ostream& outputStream); 
    protected:
      void OnStart() override;
      void ReleaseAllSubscriptions() override;
    private:
      std::ostream& _outputOstream;
      rxcpp::composite_subscription _registration;
    };
  }
}
