#include "EventRepository.hpp"

void EventRepository::saveEvent(const std::string& eventName, const EventData& eventData) {
    std::lock_guard<std::mutex> lock(mutex_);
    events_[eventName].push_back(eventData);
}

std::vector<EventData> EventRepository::getEvents(const std::string& eventName,
                                                  uint64_t startTimestamp,
                                                  uint64_t endTimestamp) const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<EventData> filtered;

    auto it = events_.find(eventName);
    if (it != events_.end()) {
        for (const auto& data : it->second) {
            if (data.date >= startTimestamp && data.date <= endTimestamp) {
                filtered.push_back(data);
            }
        }
    }
    return filtered;
}
