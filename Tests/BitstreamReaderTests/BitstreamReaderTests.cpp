#include <gtest/gtest.h>
#include "System/IO/BitstreamReader.hpp"
#include <vector>

TEST(BitstreamReaderTests, ReadBitsSimple) {
    std::vector<uint8_t> buffer = {0b11010101}; // 213
    System::IO::BitstreamReader reader(buffer);
	
	
	
    // Reads 4 bits: 0101 -> 5
    EXPECT_EQ(reader.ReadBits(4), 5);
    EXPECT_EQ(reader.GetBitPosition(), 4);

    // Reads next 4 bits: 1101 -> 13
    EXPECT_EQ(reader.ReadBits(4), 13);
    EXPECT_EQ(reader.GetBitPosition(), 8);
    ASSERT_TRUE(reader.IsEOF());
}

TEST(BitstreamReaderTests, ReadAcrossByteBoundary) {
    std::vector<uint8_t> buffer = {0b10101010, 0b11001100};
    System::IO::BitstreamReader reader(buffer);

    reader.SkipBits(4); // Move to the middle of the first byte
    // Reads 8 bits, 4 from first byte (1010) and 4 from second (1100)
    // Expected: 11001010 -> 202
    EXPECT_EQ(reader.ReadBits(8), 202);
    EXPECT_EQ(reader.GetBitPosition(), 12);
}

