#include "stdafx.h"
#include "../EventProcessing/EventAggregator.h"
#include "../EventProcessing/Event.h"
#include <string>
using namespace testing;
using namespace std;
using namespace rxcpp::notifications;
using namespace rxcpp;

namespace RxEventAggregator
{
  namespace EventProcessing
  {
    class TestEvent : public Event
    {
    public:
      static const int TEST_EVENT_ID = 1;

      TestEvent(int id = TEST_EVENT_ID, const string& description = "")
        : Event(id, description)
      {
      }
    };

    class SpecialTestEvent : public Event
    {
    public:
      static const int TEST_EVENT_ID = 2;

      SpecialTestEvent(int id = TEST_EVENT_ID, const string& description = "")
        : Event(id, description)
      {
      }
    };


    TEST(EventAggregatorTest, GetEventStream_When_Called_Then_Observable_Accepts_Observer)
    {
      EventAggregator<Event> aggregator;
      auto events = aggregator.GetEventStream<Event>();
      auto subscription = events.subscribe([](auto _)
      {
      });
      subscription.unsubscribe();
      SUCCEED();
    }

    TEST(EventAggregatorTest, Publish_When_One_Event_Published_Then_Observer_Handles_One_Event)
    {
      const int EXPECTED_EVENTS = 1;
      EventAggregator<Event> aggregator;
      auto events = aggregator.GetEventStream<TestEvent>();
      int handledEventCount = 0;
      auto subscription = events.subscribe([&handledEventCount](auto _)
      {
        handledEventCount++;
      });
      aggregator.PublishEvent(make_shared<TestEvent>());
      ASSERT_EQ(handledEventCount, EXPECTED_EVENTS);
    }

    TEST(EventAggregatorTest, Publish_When_More_Events_Published_Then_Observer_Handles_All_Subscribed_Events)
    {
      const int EXPECTED_EVENTS = 10;
      EventAggregator<Event> aggregator;
      auto events = aggregator.GetEventStream<TestEvent>();
      int handledEventCount = 0;
      auto subscription = events.subscribe([&handledEventCount](auto _)
      {
        handledEventCount++;
      });

      for (auto i = 0; i < EXPECTED_EVENTS; i++)
      {
        aggregator.PublishEvent(make_shared<TestEvent>());
        aggregator.PublishEvent(make_shared<SpecialTestEvent>());
      }

      ASSERT_EQ(handledEventCount, EXPECTED_EVENTS);
    }

    TEST(EventAggregatorTest, Publish_When_More_Events_Published_Then_All_Observers_Handles_All_Subscribed_Events)
    {
      const int EXPECTED_TYPED_EVENTS = 10000;
      const int EXPECTED_ALL_EVENTS = 2 * EXPECTED_TYPED_EVENTS;
      EventAggregator<Event> aggregator;
      auto testEventsStream = aggregator.GetEventStream<TestEvent>();
      int handledTestEventCount = 0;
      testEventsStream.subscribe([&handledTestEventCount](auto _)
      {
        handledTestEventCount++;
      });

      auto specialEventsStream = aggregator.GetEventStream<SpecialTestEvent>();
      int handledSpecialTestEventCount = 0;
      specialEventsStream.subscribe([&handledSpecialTestEventCount](auto _)
      {
        handledSpecialTestEventCount++;
      });

      int handleAllEventsCount = 0;
      auto allEventsStream = aggregator
                             .GetEventStream<SpecialTestEvent>().map([](auto& event)
                             {
                               return static_pointer_cast<Event>(event);
                             })
                             .merge(aggregator.GetEventStream<TestEvent>().map([](auto& event)
                             {
                               return static_pointer_cast<Event>(event);
                             }));
      allEventsStream.subscribe([&handleAllEventsCount](auto _)
      {
        handleAllEventsCount++;
      });

      int handleAllEventsAltCount = 0;
      auto altAllEventsStream = aggregator
        .GetEventStream<Event>();

      altAllEventsStream.subscribe([&handleAllEventsAltCount](auto _)
      {
        handleAllEventsAltCount++;
      });

      for (auto i = 0; i < EXPECTED_TYPED_EVENTS; i++)
      {
        aggregator.PublishEvent(make_shared<TestEvent>());
        aggregator.PublishEvent(make_shared<SpecialTestEvent>());
      }

      ASSERT_EQ(handledTestEventCount, EXPECTED_TYPED_EVENTS);
      ASSERT_EQ(handledSpecialTestEventCount, EXPECTED_TYPED_EVENTS);
      ASSERT_EQ(handleAllEventsCount, EXPECTED_ALL_EVENTS);
      ASSERT_EQ(handleAllEventsAltCount, EXPECTED_ALL_EVENTS);
    }
  }
}
