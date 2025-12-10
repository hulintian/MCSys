#include <boost/asio.hpp>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <test/opengl_test.h>

// 简单的十六进制打印函数
void print_hex(const uint8_t* data, std::size_t len, std::size_t max_bytes = 64) {
    std::size_t n = std::min(len, max_bytes);
    for (std::size_t i = 0; i < n; ++i) {
        if (i % 16 == 0) {
            std::cout << "\n  ";
        }
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(data[i]) << " ";
    }
    if (len > max_bytes) {
        std::cout << "\n  ... (" << (len - max_bytes) << " bytes more)";
    }
    std::cout << std::dec << "\n";
}

void test() {
    // while (1) {
    //     repark_t pack;

    //     ld_udp_ptr->sync_get_repackedpack(pack);
    //     ld_udp_ptr->get_repackedpack(pack);

    //     std::cout << "At time " << pack.interval 
    //         << " have " << pack.maxdots << " dots.\n"
    //         << "    the first data is : \n"  
    //         << "        time : "        << pack.dotcloud[1110].timestamp << "\n"
    //         << "        angle : "       << pack.dotcloud[1110].angle << "\n"
    //         << "        distance : "    << pack.dotcloud[1110].distance << "\n"
    //         << "        rssi : "        << pack.dotcloud[1110].rssi << "\n";
    // }

    // try {
    //     // listen to port 2368
    //     // 用法: ./lidar_udp_dump [bind_ip] [port]
    //     // 示例: ./lidar_udp_dump 0.0.0.0 2368
    //     const char* bind_ip = "0.0.0.0";
    //     unsigned short port = 2368;

    //     if (argc >= 2) {
    //         bind_ip = argv[1];
    //     }
    //     if (argc >= 3) {
    //         port = static_cast<unsigned short>(std::stoi(argv[2]));
    //     }

    //     std::cout << "Listening UDP on " << bind_ip << ":" << port << " ..." << std::endl;

    //     boost::asio::io_context io;
    //     udp::endpoint local_endpoint(boost::asio::ip::make_address(bind_ip), port);
    //     udp::socket socket(io, local_endpoint);

    //     // 一次最多收 2048 字节，足够容纳大部分雷达一个数据包
    //     std::vector<uint8_t> buffer(2048);
    //     udp::endpoint sender_endpoint;

    //     while (true) {
    //         boost::system::error_code ec;
    //         std::size_t len = socket.receive_from(
    //             boost::asio::buffer(buffer),
    //             sender_endpoint,
    //             0,
    //             ec
    //         );

    //         if (ec) {
    //             std::cerr << "receive_from error: " << ec.message() << std::endl;
    //             continue;
    //         }

    //         std::cout << "Received " << len << " bytes from "
    //                   << sender_endpoint.address().to_string() << ":"
    //                   << sender_endpoint.port() << std::endl;

    //         // 打印前 64 字节的十六进制内容，便于你对照手册
    //         print_hex(buffer.data(), len, 64);
    //         std::cout << "----------------------------------------\n";
    //     }
    // } catch (const std::exception& e) {
    //     std::cerr << "Exception: " << e.what() << std::endl;
    //     return 1;
    // }
}
