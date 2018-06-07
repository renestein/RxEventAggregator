#pragma once
#include  <memory>
#include <type_traits>
#include <rxcpp/rx.hpp>

namespace EventProcessing
{
  template <typename TBE>
  class EventAggregator
  {
  public:

    EventAggregator();

    EventAggregator(const EventAggregator& other) = delete;
    EventAggregator(EventAggregator&& other) noexcept = delete;
    EventAggregator& operator=(const EventAggregator& other) = delete;
    EventAggregator& operator=(EventAggregator&& other) noexcept = delete;
    ~EventAggregator() = default;
    template <typename TE>
    auto GetEventStream();
    template <typename TE>
    void PublishEvent(std::shared_ptr<TE> event);
  private:
    rxcpp::subjects::subject<std::shared_ptr<TBE>> _eventSubject;
  };

  template <typename TBE>
  EventAggregator<TBE>::EventAggregator() : _eventSubject()
  {
  }

  template <typename TBE>
  template <typename TE>
  auto EventAggregator<TBE>::GetEventStream()
  {
    static_assert(std::is_base_of_v<TBE, TE>, "TE should inherit from TBE");
    return _eventSubject.get_observable()
                        .map([](auto& baseEvent)
                        {
                          return std::dynamic_pointer_cast<TE>(baseEvent);
                        })
                        .filter([](auto& typedEvent)
                        {
                          return typedEvent.get() != nullptr;
                        }).as_dynamic();
  }

  template <typename TBE>
  template <typename TE>
  void EventAggregator<TBE>::PublishEvent(std::shared_ptr<TE> event)
  {
    static_assert(std::is_base_of_v<TBE, TE>, "TE should inherit from TBE");
    _eventSubject.get_subscriber()
                 .on_next(event);
  }
}
