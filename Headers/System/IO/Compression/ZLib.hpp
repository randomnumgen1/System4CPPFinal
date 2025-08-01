#include <vector>
#include <stdexcept>
#include <iostream>
#ifndef _SYSTEM_IO_COMPRESSION_ZLIB_H
#define _SYSTEM_IO_COMPRESSION_ZLIB_H

namespace System::IO::Compression {
    class ZLib {
	private:
#pragma pack(push, 1)
		struct ZlibHeader {
			union {
				uint16_t raw;

				struct {
					// CMF byte
					uint8_t compressionMethod : 4; // Bits 0–3 (usually 8 for DEFLATE)
					uint8_t compressionInfo : 4; // Bits 4–7 (e.g. window size)

					// FLG byte
					uint8_t fcheck : 5; // Bits 0–4 (header checksum)
					uint8_t fdict : 1; // Bit 5 (preset dictionary flag)
					uint8_t flevel : 2; // Bits 6–7 (compression level)
				};
			};
		};
#pragma pack(pop)
	public:
		// Reads a specified number of bits from the data vector
		static uint32_t read_bits(const std::vector<uint8_t>& data, int& bit_position, int count) {
			uint32_t value = 0;
			for (int i = 0; i < count; ++i) {
				int byte_index = (bit_position + i) / 8;
				int bit_index = (bit_position + i) % 8;
				if (byte_index < data.size()) {
					if ((data[byte_index] >> bit_index) & 1) {
						value |= (1 << i);
					}
				}
				else {
					throw std::runtime_error("Error:  read_bits reading past file");
				}
			}
			bit_position += count;
			return value;
		}
        static std::vector<uint8_t> Decompress(const std::vector<uint8_t>& data) {
            std::vector<uint8_t> result;
			int bit_position = 0;
			ZlibHeader* header = reinterpret_cast<ZlibHeader*>(const_cast<uint8_t*>(data.data()));
			std::cout << "compressionMethod[" << header->compressionMethod << "] " << std::endl;
			 
        }
        static std::vector<uint8_t> Compress(const std::vector<uint8_t>& data) {
            std::vector<uint8_t> result;
            result.push_back(0x78);

        }
    };
}
#endif