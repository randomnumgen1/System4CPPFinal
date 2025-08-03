#include <gtest/gtest.h>
#include "System/IO/BitstreamReader.hpp"
#include <vector>

TEST(BitstreamReaderTests, ReadBitsSimpleLE) {
    std::vector<uint8_t> buffer = {0b11010101}; // 213
    System::IO::BitstreamReader reader(buffer);
	
	reader.SetBitOrder();
	
	
    // Reads 4 bits: 0101 -> 5
    EXPECT_EQ(reader.ReadBits(4), 5);
    EXPECT_EQ(reader.GetBitPosition(), 4);

    // Reads next 4 bits: 1101 -> 13
    EXPECT_EQ(reader.ReadBits(4), 13);
    EXPECT_EQ(reader.GetBitPosition(), 8);
    ASSERT_TRUE(reader.IsEOF());
}

TEST(BitstreamReaderTests, ReadBitsSimpleBE) {
    std::vector<uint8_t> buffer = {0b11010101}; // 213
    System::IO::BitstreamReader reader(buffer);
	
	reader.SetBitOrder();
	
	
    // Reads 4 bits: 0101 -> 5
    EXPECT_EQ(reader.ReadBits(4), 5);
    EXPECT_EQ(reader.GetBitPosition(), 4);

    // Reads next 4 bits: 1101 -> 13
    EXPECT_EQ(reader.ReadBits(4), 13);
    EXPECT_EQ(reader.GetBitPosition(), 8);
    ASSERT_TRUE(reader.IsEOF());
}

