#include <vector>
#include <stdexcept>
#include <iostream>
#ifndef _SYSTEM_IO_COMPRESSION_ZLIB_H
#define _SYSTEM_IO_COMPRESSION_ZLIB_H

#include "Deflate.hpp"
#include <System/IO/MemoryBinaryReader.hpp>



namespace System::IO::Compression {
	class ZLib {
	private:
		struct ZLIBHEADER{
			uint8_t CMF;
			uint8_t FLG;
			// CMF byte
			inline uint8_t GetCompressionMethod() const{
				return CMF & 0x0F;
			}
			inline uint8_t GetCompressionInfo() const{
				return (CMF >> 4) & 0x0F;
			}
			// FLG byte
			inline uint8_t GetFcheck() const{
				return FLG & 0x1F;
			}
			inline uint8_t GetFdict() const{
				return (FLG >> 5) & 1;			
			}
			inline uint8_t GetFlevel() const{
				return (FLG >> 6) & 0x03;
			}
		};
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
			BitstreamReader bitstream(data);
			std::vector<uint8_t> result;
			ZLIBHEADER zlibheader = {};
			zlibheader.CMF = (uint8_t)bitstream.ReadUInt8();
			zlibheader.FLG = (uint8_t)bitstream.ReadUInt8();

			if (((zlibheader.CMF * 256 + zlibheader.FLG) % 31) != 0) {
				throw std::runtime_error("Invalid ZLib data: header checksum failed");
			}
			if (zlibheader.GetCompressionMethod() != 8) {
				throw std::runtime_error("Invalid ZLib data: unsupported compression method");
			}
			if (zlibheader.GetFdict()) {
				throw std::runtime_error("Invalid ZLib data: preset dictionary not supported");
			}
			std::vector<uint8_t> decompressed = DeflateStream::DecompressBlock(bitstream);
			uint32_t adler32_checksum  = bitstream.ReadBits(32);


			return decompressed;
		}







		static std::vector<uint8_t> Decompress2(const std::vector<uint8_t>& data) {
			System::IO::MemoryBinaryReader memorystream(data.data(), data.size());
			std::vector<uint8_t> result;
			ZLIBHEADER zlibheader = {};
			zlibheader.CMF = memorystream.ReadByte();
			zlibheader.FLG = memorystream.ReadByte();
			if (((zlibheader.CMF * 256 + zlibheader.FLG) % 31) != 0) {
				throw std::runtime_error("Invalid ZLib data: header checksum failed");
			}
			if (zlibheader.GetCompressionMethod() != 8) {
				throw std::runtime_error("Invalid ZLib data: unsupported compression method");
			}
			if (zlibheader.GetFdict()) {
				throw std::runtime_error("Invalid ZLib data: preset dictionary not supported");
			}
			// The zlib header is 2 bytes, the adler32 checksum is 4 bytes at the end
			const uint8_t* compressed_ptr = data.data() + 2;
			size_t compressed_len = data.size() - 6; // 2 for header, 4 for footer
			return DeflateStream::Decompress(compressed_ptr, compressed_len);
		}


		static uint32_t adler32(const std::vector<uint8_t>& data) {
			uint32_t a = 1, b = 0;
			for (uint8_t byte : data) {
				a = (a + byte) % 65521;
				b = (b + a) % 65521;
			}
			return (b << 16) | a;
		}

		static std::vector<uint8_t> Compress(const std::vector<uint8_t>& data) {
			std::vector<uint8_t> result;
			// ZLib header
			result.push_back(0x78); // CMF
			result.push_back(0x01); // FLG - No preset dictionary, fastest algorithm

			// Deflate compressed data
			std::vector<uint8_t> compressed_data = DeflateStream::Compress(data, DeflateStream::CompressionLevel::Level1);
			result.insert(result.end(), compressed_data.begin(), compressed_data.end());

			// Adler32 checksum
			uint32_t checksum = adler32(data);
			result.push_back((checksum >> 24) & 0xFF);
			result.push_back((checksum >> 16) & 0xFF);
			result.push_back((checksum >> 8) & 0xFF);
			result.push_back(checksum & 0xFF);

			return result;
		}




	};
}
#endif