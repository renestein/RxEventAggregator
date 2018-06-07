#pragma once
#include <chrono>

namespace EventProcessing
{
  namespace Detail
  {
    class DateTime
    {
    public:
      DateTime(const DateTime& other) = delete;
      DateTime(DateTime&& other) noexcept = delete;
      DateTime& operator=(const DateTime& other) = delete;
      DateTime& operator=(DateTime&& other) noexcept = delete;
      static time_t GetCurrentDateTime()
      {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        return now;
      }
    };
  }
}
