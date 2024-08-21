#ifndef MD5_H
#define MD5_H
#include "../libs/config.h"


class MD5 {
public:
    MD5();
    void update(const std::string& input);
    void update(const uint8_t* input, size_t length);
    void finalize();
    std::string hexdigest() const;

private:
    void transform(const uint8_t block[64]);
    static void encode(uint8_t* output, const uint32_t* input, size_t length);
    static void decode(uint32_t* output, const uint8_t* input, size_t length);

    bool finalized;
    uint8_t buffer[64];
    uint32_t count[2];
    uint32_t state[4];
    uint8_t digest[16];

    static const uint32_t S[64];
    static const uint32_t K[64];

    static inline uint32_t F(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t G(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t H(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t I(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t rotate_left(uint32_t x, uint32_t n);
    static inline void FF(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
    static inline void GG(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
    static inline void HH(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
    static inline void II(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
};

#endif // MD5_H

