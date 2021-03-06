// RXEventAggregator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
 #include "Lib/EventProcessing/EventAggregator.h"
#include "Lib/EventProcessing/Event.h"
#include <gtest/gtest.h>
using namespace EventProcessing;
using namespace std;
using namespace testing;

int TryRunAggregator()
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

int main(int argc, char** argv)
{
  InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
  //return TryRunAggregator();
}
