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

				static uint16_t DecodeSymbol(const std::vector<uint8_t>& data, int& bit_position, HuffmanTree& tree) {
					HuffmanNode* current = tree.root;
					while (current && !current->IsLeaf()) {
						int bit = read_bits(data, bit_position, 1);
						if (bit) {
							current = current->Right;
						}
						else {
							current = current->Left;
						}
					}
					return current ? current->Symbol : -1;
				}

				static uint16_t DecodeLength(uint16_t symbol, const std::vector<uint8_t>& data, int& bit_position) {
					if (symbol < 265) {
						return symbol - 254;
					}
					if (symbol > 284) {
						return 258;
					}
					uint8_t extra_bits = length_extra[symbol - 257];
					return length_base[symbol - 257] + read_bits(data, bit_position, extra_bits);
				}

				static uint16_t DecodeDistance(uint16_t symbol, const std::vector<uint8_t>& data, int& bit_position) {
					if (symbol < 4) {
						return symbol + 1;
					}
					if (symbol > 29) {
						// error
						return 0;
					}
					uint8_t extra_bits = dist_extra[symbol];
					return dist_base[symbol] + read_bits(data, bit_position, extra_bits);
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

							uint16_t len = Deflate::read_bits(data, bit_position, 16);
							uint16_t nlen = Deflate::read_bits(data, bit_position, 16);

							if ((len ^ nlen) != 0xFFFF) {
								// Error: invalid stored block length
								return {};
							}

							for (int i = 0; i < len; ++i) {
								result.push_back(Deflate::read_bits(data, bit_position, 8));
							}
						}else if (type == Static){
						 
						}else if (type == Dynamic){
							// TODO: Implement dynamic Huffman decoding
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
