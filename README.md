# RxEventAggregator
Simple event aggregator (messenger) based on RxCpp (https://github.com/ReactiveX/RxCpp). 
Backup of the repository https://bitbucket.org/renestein/rxeventaggregator/src/master/

```
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
  ```
