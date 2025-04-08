#pragma once

#include "EventRepository.hpp"
#include <string>
#include <memory>
#include <boost/beast/http.hpp>

class EventHandler {
public:
    explicit EventHandler(std::shared_ptr<EventRepository> repository);

    boost::beast::http::response<boost::beast::http::string_body>
    handlePost(const std::string& eventName, const std::string& body) const;

    boost::beast::http::response<boost::beast::http::string_body>
    handleGet(const std::string& eventName, const std::string& body) const;

private:
    std::shared_ptr<EventRepository> repository_;
};
