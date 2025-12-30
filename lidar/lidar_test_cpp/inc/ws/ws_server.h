// ws/ws_server.h
#pragma once

#include <boost/asio.hpp>
#include <unordered_set>
#include <mutex>
#include <memory>

#include "ws_session.h"
#include "ws_types.h"

namespace wslib {

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class WsServer {
public:
    WsServer(asio::io_context& ioc, uint16_t port);

    void run();

    // 给所有在线客户端广播
    void broadcast(ByteBufferPtr data);

private:
    void do_accept();

private:
    struct WeakHash {
        size_t operator()(const std::weak_ptr<WsSession>& w) const noexcept {
            return std::hash<const void*>()(w.lock().get());
        }
    };
    struct WeakEq {
        bool operator()(const std::weak_ptr<WsSession>& a,
                        const std::weak_ptr<WsSession>& b) const noexcept {
            return a.lock().get() == b.lock().get();
        }
    };

    asio::io_context& ioc_;
    tcp::acceptor acceptor_;

    std::mutex m_;
    std::unordered_set<
        std::shared_ptr<WsSession>
    > sessions_;
};

} // namespace wslib
