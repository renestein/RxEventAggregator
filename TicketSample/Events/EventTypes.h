#pragma once
namespace TicketApp
{
  namespace Events
  {
    const unsigned long long APP_EVENT_NONE = 0;
    const unsigned long long APP_EVENT_MESSAGE = 1 << 0;
    const unsigned long long APP_EVENT_ERROR_OCCURED = 1 << 1;
    const unsigned long long APP_EVENT_PAYMENT_RECEIVED = 1 << 2;
    const unsigned long long APP_EVENT_RESTART = 1 << 3;
    const unsigned long long APP_EVENT_TICKET_SELECTED = 1 << 4;
    const unsigned long long APP_EVENT_PAYMENT_REQUEST = 1 << 5;
    const unsigned long long APP_EVENT_PRINT_TICKET_REQUEST = 1 << 6;
    const unsigned long long APP_EVENT_PRINT_TICKET_RESPONSE = 1 << 7;
    const unsigned long long APP_EVENT_CANCELED = 1 << 8;
    const unsigned long long APP_EVENT_QUIT = 1 << 9;

    const unsigned long long APP_EVENT_ALL = ~APP_EVENT_NONE;
  }
}