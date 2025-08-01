#include <gtest/gtest.h>
#include "System/IO/Compression/ZLib.hpp"
#include "System/IO/File.h"

TEST(ZLibTests, ZLibDecompressFile) {
    // Assuming the test will be run from a subdirectory in Tests, like Tests/Compression
    std::vector<uint8_t> decompressed_data = System::IO::Compression::ZLib::Decompress(System::IO::File::ReadAllBytes("hello.zlib"));
    std::string decompressed_string(decompressed_data.begin(), decompressed_data.end());
	ASSERT_EQ(decompressed_string, std::string("Hello ZLib!"));
}
//decompresses the data generated from generate_zlib2.py
TEST(ZLibTests, ZLibDecompressMaxCompressedFile) {
    std::vector<uint8_t> decompressed_data = System::IO::Compression::ZLib::Decompress(System::IO::File::ReadAllBytes("hello.zlib"));
    std::string decompressed_string(decompressed_data.begin(), decompressed_data.end());
    std::string expected_string;
    for (int i = 0; i < 100; ++i) {
        expected_string += "The quick brown fox jumps over the lazy dog. ";
    }
    for (int i = 0; i < 100; ++i) {
        expected_string += "Pack my box with five dozen liquor jugs. ";
    }
    ASSERT_EQ(decompressed_string, expected_string);
}
