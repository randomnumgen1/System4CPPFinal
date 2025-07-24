#include <gtest/gtest.h>
#include "System/IO/Compression/Deflate.hpp"
#include "System/IO/File.h"

//The first block is a fixed huffman block, and the second is a stored block. The first block is not the last block, and the second block is the last block. 
TEST(DeflateTests, DeflateDecompressFile) {
	System::IO::Compression::Deflate deflate;
    std::vector<uint8_t> decompressed_data = System::IO::Compression::Deflate::Decompress(System::IO::File::ReadAllBytes("compressed.deflate"));
	if (std::string(decompressed_data.begin(), decompressed_data.end()) == "Hello, World!") {
		SUCCEED();
	}else{
		FAIL();
		
	}
	
}
 