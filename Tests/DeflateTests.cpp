#include <gtest/gtest.h>
#include "System/IO/Compression/Deflate.hpp"
#include "System/IO/File.h"


TEST(DeflateTests, DeflateDecompressFile) {
	System::IO::Compression::Deflate deflate;
    std::vector<uint8_t> decompressed_data = System::IO::Compression::Deflate::Decompress(System::IO::File::ReadAllBytes("compressed.deflate"));
	if (std::string(decompressed_data.begin(), decompressed_data.end()) == "Hello, World!") {
		SUCCEED();
	}else{
		FAIL();
		
	}
	
}
 