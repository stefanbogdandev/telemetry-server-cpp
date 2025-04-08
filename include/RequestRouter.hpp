#pragma once

#include "EventHandler.hpp"
#include <boost/beast/http.hpp>
#include <memory>

class RequestRouter {
public:
    using Request = boost::beast::http::request<boost::beast::http::string_body>;
    using Response = boost::beast::http::response<boost::beast::http::string_body>;

    explicit RequestRouter(std::shared_ptr<EventHandler> handler);

    Response route(Request&& req);

private:
    std::shared_ptr<EventHandler> handler_;
};
