#pragma once
#include <cassert>
#include <cstring>
#include <cstdint>
namespace System::Security::Cryptography::HashAlgorithm {
                const size_t SHA256_BLOCK_SIZE = 64; // Block size for SHA-256
                const size_t SHA256_DIGEST_SIZE = 32; // Digest size for SHA-256

                class SHA256 {
                public:
                    alignas(16) uint32_t state[8]; // 8 State variables
                    alignas(16) uint8_t buffer[SHA256_BLOCK_SIZE];
                    uint64_t count; // Number of bits processed

                    SHA256() {
                        reset();
                    }

                    void update(const uint8_t* data, size_t len) {
                        size_t bufferIndex = (count >> 3) & 0x3F;
                        count += static_cast<uint64_t>(len) << 3;
                        size_t space = SHA256_BLOCK_SIZE - bufferIndex;
                        size_t i = 0;

                        if (len >= space) {
                            std::memcpy(buffer + bufferIndex, data, space);
                            processBlock(buffer);
                            for (i = space; i + SHA256_BLOCK_SIZE <= len; i += SHA256_BLOCK_SIZE) {
                                processBlock(data + i);
                            }
                            bufferIndex = 0;
                        }
                        std::memcpy(buffer + bufferIndex, data + i, len - i);
                    }

                    void finalize(uint8_t hash[SHA256_DIGEST_SIZE]) {
                        // Padding
                        size_t bufferIndex = (count >> 3) & 0x3F;
                        buffer[bufferIndex++] = 0x80;
                        if (bufferIndex > 56) {
                            std::memset(buffer + bufferIndex, 0, SHA256_BLOCK_SIZE - bufferIndex);
                            processBlock(buffer);
                            bufferIndex = 0;
                        }
                        std::memset(buffer + bufferIndex, 0, 56 - bufferIndex);

                        // Append length in bits
                        uint64_t totalBits = count;
                        for (int i = 0; i < 8; i++) {
                            buffer[SHA256_BLOCK_SIZE - 1 - i] = static_cast<uint8_t>(totalBits >> (8 * i));
                        }
                        processBlock(buffer);

                        // Produce the final hash value
                        for (size_t i = 0; i < 8; i++) {
                            hash[i * 4] = static_cast<uint8_t>(state[i] >> 24);
                            hash[i * 4 + 1] = static_cast<uint8_t>(state[i] >> 16);
                            hash[i * 4 + 2] = static_cast<uint8_t>(state[i] >> 8);
                            hash[i * 4 + 3] = static_cast<uint8_t>(state[i]);
                        }

                        reset();
                    }

                private:
                    void reset() {
                        count = 0;
                        state[0] = 0x6a09e667;
                        state[1] = 0xbb67ae85;
                        state[2] = 0x3c6ef372;
                        state[3] = 0xa54ff53a;
                        state[4] = 0x510e527f;
                        state[5] = 0x9b05688c;
                        state[6] = 0x1f83d9ab;
                        state[7] = 0x5be0cd19;
                    }

                    void processBlock(const uint8_t* data) {
                        static const uint32_t k[] = {
                            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
                        };

                        uint32_t w[64];
                        for (size_t i = 0; i < 16; ++i) {
                            w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) |
                                (data[i * 4 + 2] << 8) | (data[i * 4 + 3]);
                        }
                        for (size_t i = 16; i < 64; ++i) {
                            w[i] = sigma1(w[i - 2]) + w[i - 7] + sigma0(w[i - 15]) + w[i - 16];
                        }

                        uint32_t a = state[0];
                        uint32_t b = state[1];
                        uint32_t c = state[2];
                        uint32_t d = state[3];
                        uint32_t e = state[4];
                        uint32_t f = state[5];
                        uint32_t g = state[6];
                        uint32_t h = state[7];

                        for (size_t i = 0; i < 64; ++i) {
                            uint32_t temp1 = h + Sigma1(e) + Ch(e, f, g) + k[i] + w[i];
                            uint32_t temp2 = Sigma0(a) + Maj(a, b, c);
                            h = g;
                            g = f;
                            f = e;
                            e = d + temp1;
                            d = c;
                            c = b;
                            b = a;
                            a = temp1 + temp2;
                        }

                        state[0] += a;
                        state[1] += b;
                        state[2] += c;
                        state[3] += d;
                        state[4] += e;
                        state[5] += f;
                        state[6] += g;
                        state[7] += h;
                    }

                    uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) const {
                        return (x & y) ^ (~x & z);
                    }

                    uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) const {
                        return (x & y) ^ (x & z) ^ (y & z);
                    }

                    uint32_t Sigma0(uint32_t x) const {
                        return rightRotate(x, 2) ^ rightRotate(x, 13) ^ rightRotate(x, 22);
                    }

                    uint32_t Sigma1(uint32_t x) const {
                        return rightRotate(x, 6) ^ rightRotate(x, 11) ^ rightRotate(x, 25);
                    }

                    uint32_t sigma0(uint32_t x) const {
                        return rightRotate(x, 7) ^ rightRotate(x, 18) ^ (x >> 3);
                    }

                    uint32_t sigma1(uint32_t x) const {
                        return rightRotate(x, 17) ^ rightRotate(x, 19) ^ (x >> 10);
                    }

                    uint32_t rightRotate(uint32_t x, size_t c) const {
                        return (x >> c) | (x << (32 - c));
                    }


                };
            }
        