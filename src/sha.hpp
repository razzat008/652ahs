#ifndef SHA_H
#define SHA_H

#include "../libs/config.h"

class SHA256 {
public:
    SHA256();
    void update(const std::string &data);
    std::string digest();

private:
    static constexpr size_t BlockSize = 64; // 512 bits
    static constexpr size_t HashValues = 8; // 256 bits (8 * 32-bit words)
    void processBlock(const uint8_t block[BlockSize]);
    void finalize();
    static uint32_t rotr(uint32_t x, uint32_t n);
    static uint32_t choose(uint32_t e, uint32_t f, uint32_t g);
    static uint32_t majority(uint32_t a, uint32_t b, uint32_t c);
    static uint32_t sigma0(uint32_t x);
    static uint32_t sigma1(uint32_t x);
    static uint32_t gamma0(uint32_t x);
    static uint32_t gamma1(uint32_t x);
    void padMessage();

    std::vector<uint8_t> dataBuffer;
    uint64_t bitLength;
    uint32_t h[HashValues];
};

#endif // SHA_H
