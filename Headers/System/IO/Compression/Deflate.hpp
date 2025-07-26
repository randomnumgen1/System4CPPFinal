#ifndef _SYSTEM_IO_COMPRESSION_DEFLATE_H
#define _SYSTEM_IO_COMPRESSION_DEFLATE_H

#include <vector>
#include <cstdint>
#include "Huffman.hpp"
namespace System {
	namespace IO {
		namespace Compression {
			class DeflateHuffmanTree {
			public:
				struct Node {
					int Symbol;
					int Left;
					int Right;
				};

				std::vector<Node> tree;

				DeflateHuffmanTree(const std::vector<int>& code_lengths) {
					tree.push_back({ -1, -1, -1 });

					int max_length = 0;
					for (int length : code_lengths) {
						if (length > max_length) {
							max_length = length;
						}
					}

					std::vector<int> bl_count(max_length + 1, 0);
					for (int length : code_lengths) {
						if (length > 0) {
							bl_count[length]++;
						}
					}

					std::vector<int> next_code(max_length + 1, 0);
					int code = 0;
					for (int bits = 1; bits <= max_length; bits++) {
						code = (code + bl_count[bits - 1]) << 1;
						next_code[bits] = code;
					}

					for (int i = 0; i < code_lengths.size(); i++) {
						int len = code_lengths[i];
						if (len != 0) {
							int current_code = next_code[len];
							next_code[len]++;
							int currentNode = 0;
							for (int j = 0; j < len; j++) {
								bool bit = (current_code >> (len - 1 - j)) & 1;
								if (bit) {
									if (tree[currentNode].Right == -1) {
										tree[currentNode].Right = tree.size();
										tree.push_back({ -1, -1, -1 });
									}
									currentNode = tree[currentNode].Right;
								}
								else {
									if (tree[currentNode].Left == -1) {
										tree[currentNode].Left = tree.size();
										tree.push_back({ -1, -1, -1 });
									}
									currentNode = tree[currentNode].Left;
								}
							}
							tree[currentNode].Symbol = i;
						}
					}
				}

				int decode(const std::vector<uint8_t>& data, int& bit_position) const {
					int currentNode = 0;
					int current_byte = data[bit_position / 8];
					int bit_offset = bit_position % 8;

					while (tree[currentNode].Left != -1) {
						int bit = (current_byte >> bit_offset) & 1;
						bit_offset++;
						if (bit_offset == 8) {
							bit_offset = 0;
							current_byte = data[bit_position / 8 + 1];
						}
						bit_position++;
						if (bit) {
							currentNode = tree[currentNode].Right;
						}
						else {
							currentNode = tree[currentNode].Left;
						}
					}
					return tree[currentNode].Symbol;
				}
				void generate_codes(const std::vector<int>& code_lengths, std::map<int, std::vector<bool>>& codes) {
					int max_length = 0;
					for (int length : code_lengths) {
						if (length > max_length) {
							max_length = length;
						}
					}

					std::vector<int> bl_count(max_length + 1, 0);
					for (int length : code_lengths) {
						if (length > 0) {
							bl_count[length]++;
						}
					}

					std::vector<int> next_code(max_length + 1, 0);
					int code = 0;
					for (int bits = 1; bits <= max_length; bits++) {
						code = (code + bl_count[bits - 1]) << 1;
						next_code[bits] = code;
					}

					for (int i = 0; i < code_lengths.size(); i++) {
						int len = code_lengths[i];
						if (len != 0) {
							int current_code = next_code[len];
							next_code[len]++;
							std::vector<bool> code_vec;
							for (int j = 0; j < len; j++) {
								code_vec.push_back((current_code >> (len - 1 - j)) & 1);
							}
							codes[i] = code_vec;
						}
					}
				}
			private:

			};
			// Deflate class for handling Deflate compression and decompression
			class Deflate {
			private:
				// Constants for Deflate algorithm
				static const int MAX_LITERALS = 288;
				static const int MAX_DISTANCES = 32;
				static const int END_OF_BLOCK = 256;
				static const int LENGTH_CODES_START = 257;
				static const int NUM_LENGTH_CODES = 29;
				static const int NUM_DISTANCE_CODES = 30;
				static const int CODE_LENGTH_CODES = 19;
				// Enum for block types in Deflate stream
				enum BlockType {
					Stored = 0b00,
					Static = 0b01,
					Dynamic = 0b10,
					Reserved = 0b11
				};
				// Enum for block markers in Deflate stream
				enum BlockMarker {
					Last = 1,
					More = 0
				}; 
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
						}else{
							throw std::runtime_error("Error:  read_bits reading past file");
						}
					}
					bit_position += count;
					return value;
				}
				// Writes a specified number of bits to the data vector
				static void write_bits(std::vector<uint8_t>& data, int& bit_position, uint32_t value, int count) {
					for (int i = 0; i < count; ++i) {
						int byte_index = (bit_position + i) / 8;
						int bit_index = (bit_position + i) % 8;
						if (byte_index >= data.size()) {
							data.resize(byte_index + 1, 0);
						}
						if ((value >> i) & 1) {
							data[byte_index] |= (1 << bit_index);
						}
					}
					bit_position += count;
				}

			public:
				// Enum for compression levels
				 enum CompressionLevel{
					 Level0,
					 Level1,
					 Level2,
					 Level3,
					 Level4,
					 Level5,
					 Level6,
					 Level7,
					 Level8,
					 Level9,

				 };
				static std::vector<uint8_t> Compress(const std::vector<uint8_t>& data, CompressionLevel compressionlevel) {
					std::vector<uint8_t> result;
					int bit_position = 0;
					write_bits(result, bit_position, (uint32_t)BlockMarker::Last, 1);
					if (compressionlevel == CompressionLevel::Level0){
						// Stored block
						write_bits(result, bit_position, (uint32_t)BlockType::Stored, 2);
						// Skip to the next byte boundary
						bit_position = (bit_position + 7) & ~7;
						uint16_t len = data.size();
						uint16_t nlen = ~len;
						write_bits(result, bit_position, len, 16);
						write_bits(result, bit_position, nlen, 16);
						for (uint8_t byte : data){
							write_bits(result, bit_position, byte, 8);
						}
					}else if(compressionlevel == CompressionLevel::Level1){
						// Static Huffman block
						write_bits(result, bit_position, (uint32_t)BlockType::Static, 2);
						std::vector<int> literal_lengths(MAX_LITERALS, 0);
						for (int i = 0; i <= 143; ++i) literal_lengths[i] = 8;
						for (int i = 144; i <= 255; ++i) literal_lengths[i] = 9;
						for (int i = 256; i <= 279; ++i) literal_lengths[i] = 7;
						for (int i = 280; i <= 287; ++i) literal_lengths[i] = 8;

						std::map<int, std::vector<bool>> literal_codes;
						DeflateHuffmanTree(literal_lengths).generate_codes(literal_lengths, literal_codes);

						for (uint8_t byte : data) {
							const auto& code = literal_codes[byte];
							for (bool bit : code) {
								write_bits(result, bit_position, bit, 1);
							}
						}
						const auto& code = literal_codes[END_OF_BLOCK];
						for (bool bit : code){
							write_bits(result, bit_position, bit, 1);
						}
					}else{
						throw std::runtime_error("Error: unsupported compression level");
					}


					return result;
				}
				/// <summary>
				/// Reads and processes a stored block from Deflate-compressed data.
				/// Stored blocks are the simplest DEFLATE block type - no compression,
				/// no Huffman codes, just raw data and integrity checks via LEN and NLEN.
				/// </summary>
				/// <param name="result"></param>
				/// <param name="data"></param>
				/// <param name="bit_position"></param>
				static void ReadStoredBlock(std::vector<uint8_t>& result, const std::vector<uint8_t>& data, int& bit_position) {
					// Skip to the next byte boundary
					bit_position = (bit_position + 7) & ~7;
					int byte_position = bit_position / 8;
					// Read length and negated length
					uint16_t len = *(uint16_t*)(data.data() + byte_position);
					byte_position += 2;
					uint16_t nlen = *(uint16_t*)(data.data() + byte_position);
					byte_position += 2;
					bit_position += 32;
					// Check if length and negated length are complements
					if ((len ^ nlen) != 0xFFFF) {
						throw std::runtime_error("Error: invalid stored block length");
					}
					// straight copy data as we are on a byte boundary
					byte_position = bit_position / 8;
					result.insert(result.end(), data.begin() + byte_position, data.begin() + byte_position + len);
					bit_position += len * 8;
				}
				/// <summary>
				/// Reads and processes a fixed Huffman block from Deflate-compressed data.
				/// Fixed blocks (also called static blocks) use a predefined literal/length Huffman tree
				/// which is specified by the Deflate RFC, enabling fast decoding without custom tree parsing.
				/// </summary>
				/// <param name="result"></param>
				/// <param name="data"></param>
				/// <param name="bit_position"></param>
				static void ReadStaticBlock(std::vector<uint8_t>& result, const std::vector<uint8_t>& data, int& bit_position){
					std::vector<int> literal_lengths(MAX_LITERALS, 0);
					for (int i = 0; i <= 143; ++i) literal_lengths[i] = 8;
					for (int i = 144; i <= 255; ++i) literal_lengths[i] = 9;
					for (int i = 256; i <= 279; ++i) literal_lengths[i] = 7;
					for (int i = 280; i <= 287; ++i) literal_lengths[i] = 8;
					DeflateHuffmanTree literal_huffman(literal_lengths);

					std::vector<int> distance_lengths(MAX_DISTANCES, 5);
					DeflateHuffmanTree distance_huffman(distance_lengths);

					static constexpr int length_extra_bits[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
					static constexpr int length_starts[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
					static constexpr int distance_extra_bits[] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
					static constexpr int distance_starts[] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };


					while (true) {
						int symbol = literal_huffman.decode(data, bit_position);
						if (symbol < END_OF_BLOCK) {
							result.push_back(symbol);
						}
						else if (symbol == END_OF_BLOCK) {
							break;
						}
						else {
							int length = length_starts[symbol - LENGTH_CODES_START] + read_bits(data, bit_position, length_extra_bits[symbol - LENGTH_CODES_START]);

							int distance_symbol = distance_huffman.decode(data, bit_position);
							int distance = distance_starts[distance_symbol] + read_bits(data, bit_position, distance_extra_bits[distance_symbol]);

							int start = result.size() - distance;
							for (int i = 0; i < length; ++i) {
								result.push_back(result[start + i]);
							}
						}
					}
				}
				/// <summary>
				/// Reads and processes a dynamic Huffman block from Deflate-compressed data.
				/// Dynamic blocks define their own Huffman trees for literals/lengths and distances,
				/// enabling efficient compression tailored to the input stream.
				/// </summary>
				/// <param name="result"></param>
				/// <param name="data"></param>
				/// <param name="bit_position"></param>
				static void ReadDynamicBlock(std::vector<uint8_t>& result, const std::vector<uint8_t>& data, int& bit_position){
					uint16_t hlit = read_bits(data, bit_position, 5) + 257;
					uint16_t hdist = read_bits(data, bit_position, 5) + 1;
					uint16_t hclen = read_bits(data, bit_position, 4) + 4;
					std::cout << "hlit: " << hlit << ", hdist: " << hdist << ", hclen: " << hclen << std::endl;

					std::vector<int> code_lengths_lengths(19, 0);
					int
						code_lengths_order[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

					for (int i = 0; i < hclen; ++i) {
						code_lengths_lengths[code_lengths_order[i]] = read_bits(data, bit_position, 3);
					}

					DeflateHuffmanTree code_lengths_huffman(code_lengths_lengths);

					std::vector<int> literal_lengths;
					while (literal_lengths.size() < hlit + hdist) {
						int symbol = code_lengths_huffman.decode(data, bit_position);
						if (symbol < 16) {
							literal_lengths.push_back(symbol);
						}
						else if (symbol == 16) {
							int repeat_count = read_bits(data, bit_position, 2) + 3;
							int previous_length = literal_lengths.back();
							for (int i = 0; i < repeat_count; ++i) {
								literal_lengths.push_back(previous_length);
							}
						}
						else if (symbol == 17) {
							int repeat_count = read_bits(data, bit_position, 3) + 3;
							for (int i = 0; i < repeat_count; ++i) {
								literal_lengths.push_back(0);
							}
						}
						else if (symbol == 18) {
							int repeat_count = read_bits(data, bit_position, 7) + 11;
							for (int i = 0; i < repeat_count; ++i) {
								literal_lengths.push_back(0);
							}
						}
					}

					std::vector<int> literal_huffman_lengths(literal_lengths.begin(), literal_lengths.begin() + hlit);
					DeflateHuffmanTree literal_huffman(literal_huffman_lengths);

					std::vector<int> distance_huffman_lengths(literal_lengths.begin() + hlit, literal_lengths.end());
					DeflateHuffmanTree distance_huffman(distance_huffman_lengths);

					while (true) {
						int symbol = literal_huffman.decode(data, bit_position);
						if (symbol < 256) {
							result.push_back(symbol);
						}
						else if (symbol == 256) {
							break;
						}
						else {
							int length_extra_bits[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
							int length_starts[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
							int length = length_starts[symbol - 257] + read_bits(data, bit_position, length_extra_bits[symbol - 257]);

							int distance_symbol = distance_huffman.decode(data, bit_position);
							int distance_extra_bits[] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
							int distance_starts[] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };
							int distance = distance_starts[distance_symbol] + read_bits(data, bit_position, distance_extra_bits[distance_symbol]);

							int start = result.size() - distance;
							for (int i = 0; i < length; ++i) {
								result.push_back(result[start + i]);
							}
						}
					}
				}


				// Decompresses data using the Deflate algorithm
				static std::vector<uint8_t> Decompress(const std::vector<uint8_t>& data) {
					std::vector<uint8_t> result;
					int bit_position = 0;

					while (true) {
						BlockMarker marker = (BlockMarker)Deflate::read_bits(data, bit_position, 1);
						BlockType type = (BlockType)Deflate::read_bits(data, bit_position, 2);

						if (type == BlockType::Stored) {
							std::cout << "Stored" << std::endl;
							ReadStoredBlock(result, data, bit_position);
							return result;
						}else if (type == BlockType::Static){
							std::cout << "Decompress using fixed Huffman codes (pre-agreed Huffman tree defined in the RFC)" << std::endl;
							ReadStaticBlock(result, data, bit_position);
							return result;
						}else if (type == BlockType::Dynamic){
							std::cout << "Dynamic with their own Huffman trees for literals/lengths and distances" << std::endl;
							ReadDynamicBlock(result, data, bit_position);
							return result;
						}else{
							throw std::runtime_error("Error, Invalid Deflate block type");
						}
						// Check if this is the last block
						if (marker == BlockMarker::Last) {
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
