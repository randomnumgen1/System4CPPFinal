#include <gtest/gtest.h>
#include "System/IO/BitstreamReader.hpp"
#include <vector>

TEST(BitstreamReaderTests, ReadBitsSimpleLE) {
    std::vector<uint8_t> buffer = {0b11010101}; // 213
    System::IO::BitstreamReader reader(buffer);
	
	reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);
	
	
    // Reads 4 bits: 0101 -> 5
    EXPECT_EQ(reader.ReadBits(4), 5);
    EXPECT_EQ(reader.GetBitPosition(), 4);

    // Reads next 4 bits: 1101 -> 13
    EXPECT_EQ(reader.ReadBits(4), 13);
    EXPECT_EQ(reader.GetBitPosition(), 8);
    ASSERT_TRUE(reader.IsEOF());
}

TEST(BitstreamReaderTests, ReadAcrossByteBoundaryLE) {
    std::vector<uint8_t> buffer = {0b10101010, 0b11001100};
    System::IO::BitstreamReader reader(buffer);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    reader.SkipBits(4); // Move to the middle of the first byte
    // Reads 8 bits, 4 from first byte (1010) and 4 from second (1100)
    // Expected: 11001010 -> 202
    EXPECT_EQ(reader.ReadBits(8), 202);
    EXPECT_EQ(reader.GetBitPosition(), 12);
}

TEST(BitstreamReaderTests, ReadAcrossByteBoundaryBE) {
    std::vector<uint8_t> buffer = {0b10101010, 0b11001100};
    System::IO::BitstreamReader reader(buffer);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::MSB0);

    reader.SkipBits(4); // Move to the middle of the first byte
    // Reads 8 bits, 4 from first byte (1010) and 4 from second (1100)
    // Expected: 10101100 -> 172
    EXPECT_EQ(reader.ReadBits(8), 172);
    EXPECT_EQ(reader.GetBitPosition(), 12);
}

TEST(BitstreamReaderTests, ReadFullByte) {
    std::vector<uint8_t> buffer = {0x42}; // 'B'
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.ReadBits(8), 0x42);
}

TEST(BitstreamReaderTests, ReadVaryingBitCountLE) {
    std::vector<uint8_t> buffer = {0b11010101};
    System::IO::BitstreamReader reader(buffer);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    EXPECT_EQ(reader.ReadBits(3), 5); // 101
    EXPECT_EQ(reader.ReadBits(5), 26); // 11010
}

TEST(BitstreamReaderTests, ReadVaryingBitCountBE) {
    std::vector<uint8_t> buffer = {0b11010101};
    System::IO::BitstreamReader reader(buffer);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::MSB0);

    EXPECT_EQ(reader.ReadBits(3), 6); // 110
    EXPECT_EQ(reader.ReadBits(5), 21); // 10101
}

TEST(BitstreamReaderTests, ReadBitsInvalidCount) {
    std::vector<uint8_t> buffer = {0x00};
    System::IO::BitstreamReader reader(buffer);

    EXPECT_THROW(reader.ReadBits(0), std::invalid_argument);
    EXPECT_THROW(reader.ReadBits(33), std::invalid_argument);
}

TEST(BitstreamReaderTests, ReadBitsSimpleBE) {
    std::vector<uint8_t> buffer = {0b11010101}; // 213
    System::IO::BitstreamReader reader(buffer);
	
	reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::MSB0);
	
	
    // Reads 4 bits: 1101 -> 13
    EXPECT_EQ(reader.ReadBits(4), 13);
    EXPECT_EQ(reader.GetBitPosition(), 4);

    // Reads next 4 bits: 0101 -> 5
    EXPECT_EQ(reader.ReadBits(4), 5);
    EXPECT_EQ(reader.GetBitPosition(), 8);
    ASSERT_TRUE(reader.IsEOF());
}


TEST(BitstreamReaderTests, EdgeCase_Read7BitsThenByteLE) {
    std::vector<uint8_t> buffer = {0b10101010, 0b11001100};
    System::IO::BitstreamReader reader(buffer);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    // Read 7 bits: 0101010
    EXPECT_EQ(reader.ReadBits(7), 0b0101010);
    // Read 8 bits: 10011001
    EXPECT_EQ(reader.ReadBits(8), 153);
    EXPECT_EQ(reader.GetBitPosition(), 15);
}
TEST(BitstreamReaderTests, EdgeCase_Read7BitsThenByteLE2) {
    std::vector<uint8_t> buffer = {0b01111111, 0b10000000};
    System::IO::BitstreamReader reader(buffer);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    // Read 7 bits
    EXPECT_EQ(reader.ReadBits(7), 0b1111111);
	EXPECT_EQ(reader.ReadBits(8), 0b00000000);

    EXPECT_EQ(reader.GetBitPosition(), 15);
}

TEST(BitstreamReaderTests, EdgeCase_Read1BitsThenByteLE3) {
    std::vector<uint8_t> buffer = {0b00000001, 0b10000000};
    System::IO::BitstreamReader reader(buffer);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    // Read 1 bit
    EXPECT_EQ(reader.ReadBits(1), 0b1);
	//Read Byte
	EXPECT_EQ(reader.ReadBits(8), 0b00000000);

    EXPECT_EQ(reader.GetBitPosition(), 9);
}

TEST(BitstreamReaderTests, EdgeCase_Read1BitsThenByteLE4) {
    std::vector<uint8_t> buffer = {0b00000011, 0b10000000};
    System::IO::BitstreamReader reader(buffer);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    // Read 1 bit
    EXPECT_EQ(reader.ReadBits(1), 0b1);
	//Read Byte
	EXPECT_EQ(reader.ReadBits(8), 0b00000001);

    EXPECT_EQ(reader.GetBitPosition(), 9);
}

