#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/beast/websocket.hpp>

#include "ws_types.h"

namespace wslib {

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

class WsSession;
using OnOpen = std::function<void(std::shared_ptr<WsSession>)>;

class WsSession : public std::enable_shared_from_this<WsSession> {
public:
    explicit WsSession(tcp::socket socket, OnOpen on_open);

    void start();
    void send(ByteBufferPtr data);
    bool is_open() const;
private:
    void on_accept(beast::error_code ec);
    
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes);

    void do_write();
    void on_write(beast::error_code ec, std::size_t bytes);

private:
    websocket::stream<tcp::socket> ws_;
    beast::flat_buffer inbuf_;

    bool writing_{false};
    std::vector<ByteBufferPtr> outbox_;

    OnOpen on_open_;
};
} // namespace wslib
