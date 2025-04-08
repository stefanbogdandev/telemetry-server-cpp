#include "EventHandler.hpp"
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;
using json = nlohmann::json;

EventHandler::EventHandler(std::shared_ptr<EventRepository> repository)
    : repository_(repository) {}

http::response<http::string_body> EventHandler::handlePost(const std::string& eventName, const std::string& body) const {
    http::response<http::string_body> res{http::status::ok, 11};
    res.set(http::field::content_type, "application/json");

    try {
        auto parsed = json::parse(body);
        EventData data;
        data.values = parsed.at("values").get<std::vector<int>>();
        data.date = parsed.at("date").get<uint64_t>();

        if (data.values.size() != 10) {
            res.result(http::status::bad_request);
            res.body() = R"({"error": "Values array must contain exactly 10 elements."})";
            res.prepare_payload();
            return res;
        }

        repository_->saveEvent(eventName, data);

        res.body() = "{}"; // empty JSON response
    }
    catch (const json::parse_error& e) {
        res.result(http::status::bad_request);
        res.body() = R"({"error": "Invalid JSON format."})";
        res.prepare_payload();
        return res;
    } 
    catch (const json::out_of_range& e) {
        res.result(http::status::bad_request);
        res.body() = R"({"error": "Missing required fields."})";
        res.prepare_payload();
        return res;
    }
    catch (const std::exception& e) {
        res.result(http::status::internal_server_error);
        res.body() = std::string("{\"error\": \"Internal server error: ") + e.what() + "\"}";
        res.prepare_payload();
        return res;
    }

    res.prepare_payload();
    return res;
}

http::response<http::string_body> EventHandler::handleGet(const std::string& eventName, const std::string& body) const {
    http::response<http::string_body> res{http::status::ok, 11};
    res.set(http::field::content_type, "application/json");

    try {
        auto parsed = json::parse(body);
        std::string resultUnit = parsed.at("resultUnit");
        uint64_t startTimestamp = parsed.value("startTimestamp", 0);
        uint64_t endTimestamp = parsed.value("endTimestamp", UINT64_MAX);

        auto events = repository_->getEvents(eventName, startTimestamp, endTimestamp);

        if (events.empty()) {
            res.body() = R"({"mean": 0})";
            res.prepare_payload();
            return res;
        }

        double totalSum = 0;
        size_t count = 0;

        for (const auto& event : events) {
            for (auto v : event.values) {
                totalSum += v;
                ++count;
            }
        }

        double mean = totalSum / static_cast<double>(count);

        if (resultUnit == "milliseconds") {
            mean *= 1000.0;
        }

        json response = { {"mean", mean} };
        res.body() = response.dump(2);
    }
    catch (const json::parse_error& e) {
        res.result(http::status::bad_request);
        res.body() = R"({"error": "Invalid JSON format."})";
        res.prepare_payload();
        return res;
    } 
    catch (const json::out_of_range& e) {
        res.result(http::status::bad_request);
        res.body() = R"({"error": "Missing required fields."})";
        res.prepare_payload();
        return res;
    }
    catch (const std::exception& e) {
        res.result(http::status::internal_server_error);
        res.body() = std::string("{\"error\": \"Internal server error: ") + e.what() + "\"}";
        res.prepare_payload();
        return res;
    }

    res.prepare_payload();
    return res;
}
