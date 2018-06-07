// RXEventAggregator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "EventProcessing/EventAggregator.h"
#include "EventProcessing/Event.h"
using namespace RxEventAggregator::EventProcessing;
using namespace std;

int main()
{
  EventAggregator<Event> eventAggregator;
  eventAggregator.GetEventStream<Event>()
                 .subscribe([](auto event)
                 {
                   cout << event->ToString();
                   cout << std::this_thread::get_id;
                 });

  eventAggregator.GetEventStream<Event>()
                 .subscribe([](auto event)
                 {
                   cout << event->ToString();
                   cout << std::this_thread::get_id;
                 });

  cout << std::this_thread::get_id << endl;
  eventAggregator.PublishEvent(make_shared<Event>(1));
  eventAggregator.PublishEvent(make_shared<Event>(1));
  cin.get();
  return 0;
}
