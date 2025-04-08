#pragma once

#include "RequestRouter.hpp"
#include "EventHandler.hpp"
#include "EventRepository.hpp"
#include "Utils.hpp"
#include <memory>
#include <boost/asio.hpp>

class TelemetryServer {
public:
    TelemetryServer(boost::asio::io_context& ioc, unsigned short port);

    void run();

    DISABLE_COPY_MOVE(TelemetryServer);

private:
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<EventRepository> repository_;
    std::shared_ptr<EventHandler> handler_;
    std::shared_ptr<RequestRouter> router_;

    void doAccept();
};
