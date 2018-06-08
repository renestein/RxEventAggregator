﻿#pragma once
#include "../../RXEventAggregator/Lib/EventProcessing/Event.h"

namespace TicketApp
{
  namespace Events
  {
    class QuitEvent : public EventProcessing::Event
    {

    public:
      const static std::string DESCRIPTION;
      QuitEvent();
    
      private:

    };
  }
}
