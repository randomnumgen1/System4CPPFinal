#include <gtest/gtest.h>
#include "System/IO/Compression/Deflate.hpp"
#include "System/IO/File.h"

//The first block is a fixed huffman block, and the second is a stored block. The first block is not the last block, and the second block is the last block.
TEST(DeflateTests, DeflateDecompressFile) {
    System::IO::Compression::Deflate deflate;
    std::vector<uint8_t> decompressed_data = System::IO::Compression::Deflate::Decompress(System::IO::File::ReadAllBytes("compressedwithbom.deflate"));
    std::string decompressed_string(decompressed_data.begin(), decompressed_data.end());
	ASSERT_EQ(decompressed_string, std::string("\xEF\xBB\xBFHello, World!"));
}


TEST(DeflateTests, DeflateCompressDecompressRoundtrip) {
    std::string original_string = "Test";
    std::vector<uint8_t> original_data(original_string.begin(), original_string.end());
    std::vector<uint8_t> compressed_data = System::IO::Compression::Deflate::Compress(original_data);
    std::vector<uint8_t> decompressed_data = System::IO::Compression::Deflate::Decompress(compressed_data);
    std::string result(decompressed_data.begin(), decompressed_data.end());
    ASSERT_EQ(result, original_string);
}

TEST(DeflateTests, DeflateCompressDecompressSemiRepetitiveRoundtrip) {
    std::string original_string = "abcabcabcabcabcabcabcabcabcabc";
    std::vector<uint8_t> original_data(original_string.begin(), original_string.end());
    std::vector<uint8_t> compressed_data = System::IO::Compression::Deflate::Compress(original_data);
    std::vector<uint8_t> decompressed_data = System::IO::Compression::Deflate::Decompress(compressed_data);
    std::string result(decompressed_data.begin(), decompressed_data.end());
    ASSERT_EQ(result, original_string);
}
TEST(DeflateTests, DeflateCompressDecompressnonRepetitiveRoundtrip) {
    std::string original_string = "xqL8v#B9$m&Nf*W@1!ZuJh5rEkPtYs";
    std::vector<uint8_t> original_data(original_string.begin(), original_string.end());
    std::vector<uint8_t> compressed_data = System::IO::Compression::Deflate::Compress(original_data);
    std::vector<uint8_t> decompressed_data = System::IO::Compression::Deflate::Decompress(compressed_data);
    std::string result(decompressed_data.begin(), decompressed_data.end());
    ASSERT_EQ(result, original_string);
}
TEST(DeflateTests, DeflateCompressDecompressRoundtrip) {
    std::string original_string = "matchmatchmatchXYZmatchmatchmatch123matchmatchmatch! AnotherMatchAnotherMatchAnotherMatch matchmatchmatchXYZmatchmatchmatch123matchmatchmatch!";
    std::vector<uint8_t> original_data(original_string.begin(), original_string.end());
    std::vector<uint8_t> compressed_data = System::IO::Compression::Deflate::Compress(original_data);
    std::vector<uint8_t> decompressed_data = System::IO::Compression::Deflate::Decompress(compressed_data);
    std::string result(decompressed_data.begin(), decompressed_data.end());
    ASSERT_EQ(result, original_string);
}


