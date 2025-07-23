//   System::Security::Cryptography::HashAlgorithm::CRC32


#pragma once
/*

CRC-32/ISO-HDLC (CRC-32, CRC-32/ADCCP, CRC-32/V-42, CRC-32/XZ, PKZIP)

CRC-32/ISCSI (CRC-32/BASE91-C, CRC-32/CASTAGNOLI, CRC-32/INTERLAKEN, CRC-32C, CRC-32/NVME)
*/

#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <array>
#include <iostream>
#include <vector>
#include <cassert>
namespace System {
    namespace Security {
        namespace Cryptography {
            namespace HashAlgorithm {
                enum CRC32POLYNOMIAL {
                    NONE = 0,
                    /// <summary>
                    /// CRC-32/ISO-HDLC (CRC-32, CRC-32/ADCCP, CRC-32/V-42, CRC-32/XZ, PKZIP)
                    /// </summary>
                    ISO_HDLC = 1,
                    /// <summary>
                    /// CRC-32/ISCSI (CRC-32/BASE91-C, CRC-32/CASTAGNOLI, CRC-32/INTERLAKEN, CRC-32C, CRC-32/NVME)
                    /// </summary>
                    ISCSI = 2
                };
                constexpr size_t CRC32_DIGEST_SIZE = 4;

                class CRC32 {
                private:
                    struct {
                        std::vector<uint32_t> table; 
                        uint32_t hash;
                    } state;
                    CRC32POLYNOMIAL  crc32polynomial;



                public:
                    CRC32() {
                        crc32polynomial = CRC32POLYNOMIAL::NONE;
                        reset();
                    }
                    CRC32(CRC32POLYNOMIAL polynomial) {
                      //  generateTable(); // Generate CRC table
                        setMode( polynomial);
                        reset();
                    }
                    void setMode(CRC32POLYNOMIAL polynomial) {
                        crc32polynomial = polynomial;
                    }
                    uint32_t crc32_u8(uint32_t prevcrc, uint8_t v) {
                        uint32_t crc = prevcrc;
                        crc ^= v;
                        for (int bit = 0; bit < 8; bit++) {
                            if (crc & 1)
                                crc = (crc >> 1) ^ (0xEDB88320ull);
                            else
                                crc = (crc >> 1);
                        }
                        return crc;
                    }

                 



                    void update(const uint8_t* bytes, size_t len) {
                        assert(crc32polynomial != CRC32POLYNOMIAL::NONE);
                        assert(bytes != nullptr);
                        size_t i = 0;
                        if (crc32polynomial == CRC32POLYNOMIAL::ISO_HDLC) {
                            while (len > 0) {
                                    state.hash = crc32_u8(state.hash, bytes[i]);
                                ++i;
                                --len;
                            }
                        }else {

#if defined(_WIN32) && (defined(__x86_64__) || defined(_M_X64))
                            // Process 32-bit chunks first
                            while (len >= 4) {
                                state.hash = _mm_crc32_u32(state.hash, *reinterpret_cast<const uint32_t*>(&bytes[i]));
                                i += 4;
                                len -= 4;
                            }

                            // Process remaining bytes
                            while (len > 0) {
                                state.hash = _mm_crc32_u8(state.hash, bytes[i]);
                                ++i;
                                --len;
                            }
#elif (defined(__arm__) || defined(_M_ARM))
                            // Process 32-bit chunks first
                            while (len >= 4) {
                                state.hash = __crc32cw(state.hash, *reinterpret_cast<const uint32_t*>(&bytes[i]));

                                i += 4;
                                len -= 4;
                            }

                            // Process remaining bytes
                            while (len > 0) {
                                state.hash = __crc32cb(state.hash, bytes[i]);

                                ++i;
                                --len;
                            }
#else
                            while (len > 0) {
                                    state.hash = _mm_crc32_u8(state.hash, bytes[i]);
                                ++i;
                                --len;
                            }
#endif
                        }
                    }
                    void finalize(uint8_t hashOutput[4]) {
                        assert(hashOutput != nullptr);
                        uint32_t finalizedHash = ~state.hash; // Invert the hash
                        hashOutput[0] = static_cast<uint8_t>((finalizedHash >> 24) & 0xFF);
                        hashOutput[1] = static_cast<uint8_t>((finalizedHash >> 16) & 0xFF);
                        hashOutput[2] = static_cast<uint8_t>((finalizedHash >> 8) & 0xFF);
                        hashOutput[3] = static_cast<uint8_t>(finalizedHash & 0xFF);
                        reset();
                    }
                    void reset() {
                        state.hash = 0 ^ 0xffffffff;
                    }

                private:
                    void generateTabled() {
                        state.table.resize(256);
                        for (uint32_t i = 0; i < 256; ++i) {
                            uint32_t crc = i;
                            for (int j = 0; j < 8; ++j) {
                                if (crc & 1) {
                                    crc = (crc >> 1) ^ 0xEDB88320;
                                }
                                else {
                                    crc >>= 1;
                                }
                            }
                            state.table[i] = crc;
                        }
                    }

                };

            } // namespace HashAlgorithm
        } // namespace Cryptography
    } // namespace Security
} // namespace System
