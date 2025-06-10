#include <cstdint>
#include <cstring>

namespace System {
    namespace Security {
        namespace Cryptography {
            namespace HashAlgorithm {

                static constexpr size_t XXHASH32_BLOCK_SIZE = 128; // Block size in bytes
                static constexpr size_t XXHASH32_DIGEST_SIZE = 4;   // Digest size in bytes

                class xxHash32 {
                public:
                    struct State {
                        uint32_t seed;
                        uint32_t state[4];
                        uint8_t buffer[XXHASH32_BLOCK_SIZE];
                        size_t bufferSize;
                        uint64_t totalLength;
                    } state;

                    xxHash32(uint32_t seed = 0) {
                        state.seed = seed;
                        reset();
                    }

                    void update(const void* data, size_t len) {
                        const uint8_t* bytes = static_cast<const uint8_t*>(data);
                        state.totalLength += len;

                        while (len > 0) {
                            size_t chunkSize = (state.bufferSize + len >= XXHASH32_BLOCK_SIZE) ? (XXHASH32_BLOCK_SIZE - state.bufferSize) : len;
                            std::memcpy(state.buffer + state.bufferSize, bytes, chunkSize);
                            state.bufferSize += chunkSize;
                            bytes += chunkSize;
                            len -= chunkSize;

                            if (state.bufferSize == XXHASH32_BLOCK_SIZE) {
                                processChunk(state.buffer);
                                state.bufferSize = 0;
                            }
                        }
                    }

                    void finalize(uint8_t* hash) {
                        processFinal();
                        std::memcpy(hash, &state.state[0], XXHASH32_DIGEST_SIZE);
                        reset();
                    }

                private:
                    static constexpr uint32_t PRIME1 = 0x9E3779B1;
                    static constexpr uint32_t PRIME2 = 0x85EBCA77;
                    static constexpr uint32_t PRIME3 = 0xC2B2AE3D;
                    static constexpr uint32_t PRIME4 = 0x27D4EB2F;
                    static constexpr uint32_t PRIME5 = 0x165667B1;

                    void reset() {
                        state.state[0] = state.seed + PRIME1 + PRIME2;
                        state.state[1] = state.seed + PRIME2;
                        state.state[2] = state.seed;
                        state.state[3] = state.seed - PRIME1;
                        state.bufferSize = 0;
                        state.totalLength = 0;
                    }

                    void processChunk(const uint8_t* chunk) {
                        const uint32_t* blocks = reinterpret_cast<const uint32_t*>(chunk);
                        for (size_t i = 0; i < XXHASH32_BLOCK_SIZE / sizeof(uint32_t); ++i) {
                            state.state[i % 4] += blocks[i] * PRIME2;
                            state.state[i % 4] = rotateLeft(state.state[i % 4], 13);
                            state.state[i % 4] *= PRIME1;
                        }
                    }

                    void processFinal() {
                        uint32_t hash = state.totalLength;

                        for (size_t i = 0; i < state.bufferSize; i++) {
                            hash += state.buffer[i] * PRIME5;
                            hash = rotateLeft(hash, 11) * PRIME1;
                        }

                        hash ^= hash >> 15;
                        hash *= PRIME2;
                        hash ^= hash >> 13;
                        hash *= PRIME3;
                        hash ^= hash >> 16;

                        state.state[0] = hash;
                    }

                    static uint32_t rotateLeft(uint32_t value, int shift) {
                        return (value << shift) | (value >> (32 - shift));
                    }
                };

            }
        }
    }
}