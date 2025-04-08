#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>

struct EventData {
    std::vector<int> values;  // 10 time durations
    uint64_t date;            // Unix timestamp (seconds)
};

class EventRepository {
public:
    void saveEvent(const std::string& eventName, const EventData& eventData);

    std::vector<EventData> getEvents(const std::string& eventName,
                                     uint64_t startTimestamp = 0,
                                     uint64_t endTimestamp = UINT64_MAX) const;

private:
    std::map<std::string, std::vector<EventData>> events_;
    mutable std::mutex mutex_;
};
