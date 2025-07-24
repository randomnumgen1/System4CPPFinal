#ifndef _SYSTEM_IO_COMPRESSION_DEFLATE_H
#define _SYSTEM_IO_COMPRESSION_DEFLATE_H

#include <vector>
#include <cstdint>
#include "Huffman.hpp"
namespace System {
	namespace IO {
		namespace Compression {
			class Deflate {
			private:
				enum blocktype {
					Stored = 0b00,
					Static = 0b01,
					Dynamic = 0b10,
					Reserved = 0b11
				};
				enum blockmarker {
					last = 1,
					more = 0
				}; 

			private: 

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
					}
					bit_position += count;
					return value;
				}

				 

			public:

				static std::vector<uint8_t> Compress(const std::vector<uint8_t>& data) {


				}
				static std::vector<uint8_t> Decompress(const std::vector<uint8_t>& data) {
					std::vector<uint8_t> result;
					int bit_position = 0;

					while (true) {
						blockmarker marker = (blockmarker)Deflate::read_bits(data, bit_position, 1);
						blocktype type = (blocktype)Deflate::read_bits(data, bit_position, 2);

						if (type == Stored) {
							// Skip to the next byte boundary
							bit_position = (bit_position + 7) & ~7;

							uint16_t len =  read_bits(data, bit_position, 16);
							uint16_t nlen =  read_bits(data, bit_position, 16);

							if ((len ^ nlen) != 0xFFFF) {
								// Error: invalid stored block length
								return {};
							}

							for (int i = 0; i < len; ++i) {
								result.push_back( read_bits(data, bit_position, 8));
							}
						}else if (type == Static){
						 
						}else if (type == Dynamic){
							uint16_t hlit =  read_bits(data, bit_position, 5) + 257;
							uint16_t hdist =  read_bits(data, bit_position, 5) + 1;
							uint16_t hclen = read_bits(data, bit_position, 4) + 4;

						 
						 
						   
						}else{
							throw std::runtime_error("block type, error");
						}
						if (marker == last) {
							break;
						}
					}
					return result;
				}
			};
		}
	}
}

#endif
