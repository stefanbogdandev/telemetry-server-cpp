#pragma once

#include "RequestRouter.hpp"
#include "Utils.hpp"
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(boost::asio::ip::tcp::socket socket, std::shared_ptr<RequestRouter> router);

    void start();

    DISABLE_COPY_MOVE(HttpSession);

private:
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::string_body> request_;
    std::shared_ptr<RequestRouter> router_;

    void readRequest();
    void handleRequest();
    void sendResponse(boost::beast::http::response<boost::beast::http::string_body> response);
};
