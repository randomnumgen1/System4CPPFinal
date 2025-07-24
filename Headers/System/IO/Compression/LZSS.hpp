#ifndef _SYSTEM_IO_COMPRESSION_LZSS_H
#define _SYSTEM_IO_COMPRESSION_LZSS_H

#include <vector>
#include <string>
#include <cstdint>

namespace System {
    namespace IO {
        namespace Compression {
            class LZSS {
            public:
                static std::vector<uint8_t> Compress(const std::vector<uint8_t>& data) {
                    std::vector<uint8_t> compressed_data;
                    if (data.empty()) {
                        return compressed_data;
                    }

                    const int N = 4096;
                    const int F = 18;
                    const int THRESHOLD = 2;

                    std::vector<uint8_t> text_buf(data.size() + N - F, ' ');
                    std::copy(data.begin(), data.end(), text_buf.begin() + N - F);

                    int i = N - F;
                    int data_idx = 0;

                    while (data_idx < data.size()) {
                        uint8_t flags = 0;
                        std::vector<uint8_t> block;
                        for (int bit = 0; bit < 8 && data_idx < data.size(); bit++) {
                            int match_length = 0;
                            int match_position = 0;

                            for (int j = data_idx - 1; j >= 0 && j >= data_idx - N; j--) {
                                if (data[j] == data[data_idx]) {
                                    int k = 1;
                                    while (k < F && data_idx + k < data.size() && data[j + k] == data[data_idx + k]) {
                                        k++;
                                    }
                                    if (k > match_length) {
                                        match_length = k;
                                        match_position = j;
                                    }
                                }
                            }

                            if (match_length > THRESHOLD) {
                                flags |= (1 << bit);
                                block.push_back(match_position & 0xFF);
                                block.push_back(((match_position >> 4) & 0xF0) | (match_length - (THRESHOLD + 1)));
                                data_idx += match_length;
                            }
                            else {
                                block.push_back(data[data_idx]);
                                data_idx++;
                            }
                        }
                        compressed_data.push_back(flags);
                        compressed_data.insert(compressed_data.end(), block.begin(), block.end());
                    }

                    return compressed_data;
                }

                static std::vector<uint8_t> Decompress(const std::vector<uint8_t>& compressed_data) {
                    std::vector<uint8_t> decompressed_data;
                    if (compressed_data.empty()) {
                        return decompressed_data;
                    }

                    const int N = 4096;
                    const int F = 18;
                    const int THRESHOLD = 2;

                    std::vector<uint8_t> text_buf(N, ' ');
                    int r = N - F;

                    int in_idx = 0;
                    while (in_idx < compressed_data.size()) {
                        uint8_t flags = compressed_data[in_idx++];
                        for (int bit = 0; bit < 8; bit++) {
                            if (in_idx >= compressed_data.size()) break;

                            if ((flags >> bit) & 1) {
                                int offset = compressed_data[in_idx++];
                                int length = compressed_data[in_idx++];
                                offset |= ((length & 0xF0) << 4);
                                length = (length & 0x0F) + THRESHOLD + 1;
                                for (int k = 0; k < length; k++) {
                                    uint8_t c = decompressed_data[offset + k];
                                    decompressed_data.push_back(c);
                                }
                            }
                            else {
                                uint8_t c = compressed_data[in_idx++];
                                decompressed_data.push_back(c);
                            }
                        }
                    }

                    return decompressed_data;
                }
            };
        }
    }
}
#endif