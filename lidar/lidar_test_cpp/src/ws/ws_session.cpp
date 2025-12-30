// ws/ws_session.cpp
#include "ws/ws_session.h"

namespace wslib {

WsSession::WsSession(tcp::socket socket, OnOpen on_open)
    : ws_(std::move(socket)) {}

void WsSession::start() {
    ws_.set_option(
        websocket::stream_base::timeout::suggested(beast::role_type::server)
    );

    ws_.set_option(
        websocket::stream_base::decorator([](websocket::response_type& res) {
            res.set(beast::http::field::server, "lidar-ws");
        })
    );

    ws_.async_accept(
        beast::bind_front_handler(
            &WsSession::on_accept,
            shared_from_this()
        )
    );
}

bool WsSession::is_open() const {
    return ws_.is_open();
}

void WsSession::send(ByteBufferPtr data) {
    // 投递回 io_context，保证线程安全
    asio::post(ws_.get_executor(),
        [self = shared_from_this(), data]() {
            if (!self->ws_.is_open()) return;
            self->outbox_.push_back(data);
            if (!self->writing_) {
                self->writing_ = true;
                self->do_write();
            }
        }
    );
}

void WsSession::on_accept(beast::error_code ec) {
    if (ec) {
        beast::error_code ignore;
        ws_.next_layer().shutdown(tcp::socket::shutdown_both, ignore);
        ws_.next_layer().close(ignore);
        return;
    }

    if (on_open_) {
        on_open_(shared_from_this());
    }
    do_read();
}

void WsSession::do_read() {
    ws_.async_read(
        inbuf_,
        beast::bind_front_handler(
            &WsSession::on_read,
            shared_from_this()
        )
    );
}

void WsSession::on_read(beast::error_code ec, std::size_t) {
    if (ec) {
        // 断开或错误，session 生命周期由 server 管理
        return;
    }
    inbuf_.consume(inbuf_.size());
    do_read();
}

void WsSession::do_write() {
    ws_.binary(true);
    ws_.async_write(
        asio::buffer(*outbox_.front()),
        beast::bind_front_handler(
            &WsSession::on_write,
            shared_from_this()
        )
    );
}

void WsSession::on_write(beast::error_code ec, std::size_t) {
    if (ec) return;

    outbox_.erase(outbox_.begin());
    if (!outbox_.empty()) {
        do_write();
    } else {
        writing_ = false;
    }
}

} // namespace wslib
