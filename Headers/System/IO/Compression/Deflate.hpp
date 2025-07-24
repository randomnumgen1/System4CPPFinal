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
				enum blockmarker{
					last = 1,
					more = 0
				};
			public:

				static std::vector<uint8_t> Compress(const std::vector<uint8_t>& data){
					

				}
				static std::vector<uint8_t> Decompress(const std::vector<uint8_t>& data) {
				
				}
			};
		}
	}
}

#endif
