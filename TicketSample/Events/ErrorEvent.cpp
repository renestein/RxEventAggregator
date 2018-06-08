#include "stdafx.h"
#include "ErrorEvent.h"
#include "EventTypes.h"

using namespace std;

namespace TicketApp
{
  namespace Events
  {
    ErrorEvent::ErrorEvent(const string &description) : Event(APP_EVENT_RESTART, description)
    {

    }
  }
}
