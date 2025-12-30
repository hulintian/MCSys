// ws/ws_server.cpp
#include "ws/ws_server.h"
#include <mutex>

namespace wslib {

WsServer::WsServer(asio::io_context& ioc, uint16_t port)
    : ioc_(ioc),
      acceptor_(ioc, {tcp::v4(), port}) {}

void WsServer::run() {
    do_accept();
}

void WsServer::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                auto session = std::make_shared<WsSession>(std::move(socket), 
                                                           [this](auto s) {
                                                                std::lock_guard<std::mutex> lk(m_);
                                                                sessions_.insert(std::move(s));
                                                           }
                                                           );
                {
                    std::lock_guard<std::mutex> lk(m_);
                    sessions_.insert(session);
                }
                session->start();
            }
            do_accept();
        }
    );
}

void WsServer::broadcast(ByteBufferPtr data) {
  std::lock_guard<std::mutex> lk(m_);
  for (auto it = sessions_.begin(); it != sessions_.end();) {
    auto s = *it;
    if (!s || !s->is_open()) {
      it = sessions_.erase(it);
      continue;
    }
    s->send(data);
    ++it;
  }
}

} // namespace wslib
