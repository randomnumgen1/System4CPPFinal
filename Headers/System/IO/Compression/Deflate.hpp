#ifndef _SYSTEM_IO_COMPRESSION_DEFLATE_H
#define _SYSTEM_IO_COMPRESSION_DEFLATE_H

#include <vector>
#include <cstdint>
#include "Huffman.hpp"
namespace System {
	namespace IO {
		namespace Compression {
			class Deflate {
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
