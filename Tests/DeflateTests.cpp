#include <gtest/gtest.h>
#include "System/IO/Compression/Deflate.hpp"


TEST(DeflateTests, DeflateDecompression) {
	
    std::vector<uint8_t> compressed_data = {0x7A, 0xBF, 0x7B, 0xBF, 0x47, 0x6A, 0x4E, 0x4E, 0xBE, 0x8E, 0x42, 0x78, 0x7E, 0x51, 0x4E, 0x8A, 0x22, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x03, 0x00};

    std::vector<uint8_t> decompressed_data = System::IO::Compression::Deflate::Decompress(compressed_data);
	
	
	if (std::string(decompressed_data.begin(), decompressed_data.end()) == "Hello, World!") {
		SUCCEED();
	}else{
		FAIL();
		
	}
	
	
	

}

