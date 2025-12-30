#include "utils/Encode_binary.h"
#include "sdk/LakiBeamUDP.h"

std::vector<uint8_t> encode_binary(const repark_t& snapshot) {
    uint16_t n = snapshot.maxdots;

    // header: 4(timestamp) + 2(count)
    //
    //  |  time  |
    //  |  size  |
    //  | angles |
    //  |  ...   |
    //  |  dots  |
    //  |  ...   |
    size_t total =
        sizeof(uint32_t) +
        sizeof(uint16_t) +
        n * (sizeof(uint16_t) * 2);

    std::vector<uint8_t> buf(total);
    uint8_t* p = buf.data();

    // timestamp（可选）
    uint32_t ts = snapshot.interval; // 你结构里有的话
    std::memcpy(p, &ts, sizeof(ts));
    p += sizeof(ts);

    // 点数
    std::memcpy(p, &n, sizeof(n));
    p += sizeof(n);

    // 点云
    for (uint16_t i = 0; i < n; ++i) {
        std::memcpy(p, &snapshot.dotcloud[i].angle, sizeof(uint16_t));
        p += sizeof(uint16_t);

        std::memcpy(p, &snapshot.dotcloud[i].distance, sizeof(uint16_t));
        p += sizeof(uint16_t);
    }

    return buf;
}
