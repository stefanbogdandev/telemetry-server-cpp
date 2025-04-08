#include "TelemetryServer.hpp"
#include "HttpSession.hpp"

#include <boost/beast.hpp>
#include <iostream>

using tcp = boost::asio::ip::tcp;

TelemetryServer::TelemetryServer(boost::asio::io_context& ioc, unsigned short port)
    : io_context_(ioc),
      acceptor_(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      repository_(std::make_shared<EventRepository>()),
      handler_(std::make_shared<EventHandler>(repository_)),
      router_(std::make_shared<RequestRouter>(handler_)) {}

void TelemetryServer::run() {
    doAccept();
}

void TelemetryServer::doAccept() {
    acceptor_.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
        if (!ec) {
            std::make_shared<HttpSession>(std::move(socket), router_)->start();
        }
        doAccept();
    });
}
