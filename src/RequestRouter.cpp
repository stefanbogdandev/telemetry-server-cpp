#include "RequestRouter.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

namespace http = boost::beast::http;
using json = nlohmann::json;

RequestRouter::RequestRouter(std::shared_ptr<EventHandler> handler)
    : handler_(handler) {}

RequestRouter::Response RequestRouter::route(Request&& req) {
    Response res{http::status::ok, req.version()};
    res.set(http::field::server, "TelemetryServer");
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());

    const std::string target = std::string(req.target());

    if (req.method() == http::verb::post && target.rfind("/paths/", 0) == 0) {
        std::string eventName = target.substr(7); // after "/paths/"
        return handler_->handlePost(eventName, req.body());
    } 
    else if (req.method() == http::verb::get && target.rfind("/paths/", 0) == 0 && target.find("/meanLength") != std::string::npos) {
        std::string eventName = target.substr(7, target.find("/meanLength") - 7); // between "/paths/" and "/meanLength"
        return handler_->handleGet(eventName, req.body());
    } 
    else {
        res.result(http::status::not_found);
        res.body() = R"({"error": "Route not found"})";
    }

    res.prepare_payload();
    return res;
}
