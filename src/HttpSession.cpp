#include "HttpSession.hpp"
#include "RequestRouter.hpp"
#include <boost/beast/http.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;

HttpSession::HttpSession(boost::asio::ip::tcp::socket socket, std::shared_ptr<RequestRouter> router)
    : socket_(std::move(socket)), router_(router) {}

void HttpSession::start() {
    readRequest();
}

void HttpSession::readRequest() {
    auto self = shared_from_this();
    http::async_read(socket_, buffer_, request_,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                self->handleRequest();
            }
        });
}

void HttpSession::handleRequest() {
    auto response = router_->route(std::move(request_));
    sendResponse(std::move(response));
}

void HttpSession::sendResponse(http::response<http::string_body> response) {
    auto self = shared_from_this();
    auto sp = std::make_shared<http::response<http::string_body>>(std::move(response));
    http::async_write(socket_, *sp,
        [self, sp](beast::error_code ec, std::size_t bytes_transferred) {
            self->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
        });
}
