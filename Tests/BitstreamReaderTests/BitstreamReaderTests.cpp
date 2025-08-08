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

TEST(BitstreamReaderTests, ReadUInt8_Aligned) {
    std::vector<uint8_t> buffer = {123, 45};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.ReadUInt8(), 123);
    EXPECT_EQ(reader.GetBitPosition(), 8);
    EXPECT_EQ(reader.ReadUInt8(), 45);
    EXPECT_EQ(reader.GetBitPosition(), 16);
}

TEST(BitstreamReaderTests, ReadUInt8_Unaligned) {
    std::vector<uint8_t> buffer = {0b10101010, 0b11001100};
    System::IO::BitstreamReader reader(buffer);
    reader.SkipBits(4);
    // 11001010
    EXPECT_EQ(reader.ReadUInt8(), 202);
    EXPECT_EQ(reader.GetBitPosition(), 12);
}

TEST(BitstreamReaderTests, ReadInt8_Aligned) {
    std::vector<uint8_t> buffer = {123, (uint8_t)-45};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.ReadInt8(), 123);
    EXPECT_EQ(reader.ReadInt8(), -45);
}

TEST(BitstreamReaderTests, ReadUInt16_Aligned) {
    std::vector<uint8_t> buffer = {0x12, 0x34, 0x56, 0x78};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.ReadUInt16(), 0x3412);
    EXPECT_EQ(reader.ReadUInt16(), 0x7856);
}

TEST(BitstreamReaderTests, ReadUInt16_Unaligned) {


}

TEST(BitstreamReaderTests, ReadInt16_Aligned) {
    std::vector<uint8_t> buffer = {0x12, 0x34, 0xFE, 0xFF}; // 13330, -2
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.ReadInt16(), 0x3412);
    EXPECT_EQ(reader.ReadInt16(), -2);
}

TEST(BitstreamReaderTests, ReadUInt32_Aligned) {
    std::vector<uint8_t> buffer = {0x12, 0x34, 0x56, 0x78};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.ReadUInt32(), 0x78563412);
}

TEST(BitstreamReaderTests, ReadInt32_Aligned) {
    std::vector<uint8_t> buffer = {0x12, 0x34, 0x56, 0x78, 0xFE, 0xFF, 0xFF, 0xFF};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.ReadInt32(), 0x78563412);
    EXPECT_EQ(reader.ReadInt32(), -2);
}

TEST(BitstreamReaderTests, ReadBool) {
    std::vector<uint8_t> buffer = {0b10101010};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.ReadBool(), false);
    EXPECT_EQ(reader.ReadBool(), true);
    EXPECT_EQ(reader.ReadBool(), false);
    EXPECT_EQ(reader.ReadBool(), true);
    EXPECT_EQ(reader.ReadBool(), false);
    EXPECT_EQ(reader.ReadBool(), true);
    EXPECT_EQ(reader.ReadBool(), false);
    EXPECT_EQ(reader.ReadBool(), true);
}

TEST(BitstreamReaderTests, PeekBits) {
    std::vector<uint8_t> buffer = {0b11010101};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.PeekBits(4), 5);
    EXPECT_EQ(reader.GetBitPosition(), 0);
    EXPECT_EQ(reader.ReadBits(4), 5);
    EXPECT_EQ(reader.GetBitPosition(), 4);
}

TEST(BitstreamReaderTests, SkipBits) {
    std::vector<uint8_t> buffer = {0, 0, 0, 0};
    System::IO::BitstreamReader reader(buffer);
    reader.SkipBits(10);
    EXPECT_EQ(reader.GetBitPosition(), 10);
}

TEST(BitstreamReaderTests, SkipBytes) {
    std::vector<uint8_t> buffer = {0, 0, 0, 0};
    System::IO::BitstreamReader reader(buffer);
    reader.SkipBytes(2);
    EXPECT_EQ(reader.GetBitPosition(), 16);
}

TEST(BitstreamReaderTests, AlignToByte) {
    std::vector<uint8_t> buffer = {0, 0, 0, 0};
    System::IO::BitstreamReader reader(buffer);
    reader.ReadBits(3);
    reader.AlignToByte();
    EXPECT_EQ(reader.GetBitPosition(), 8);
}

TEST(BitstreamReaderTests, IsEOF) {
    std::vector<uint8_t> buffer = {0, 0};
    System::IO::BitstreamReader reader(buffer);
    reader.ReadBits(15);
    EXPECT_FALSE(reader.IsEOF());
    reader.ReadBits(1);
    EXPECT_TRUE(reader.IsEOF());
}

TEST(BitstreamReaderTests, RemainingBits) {
    std::vector<uint8_t> buffer = {0, 0};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.RemainingBits(), 16);
    reader.ReadBits(5);
    EXPECT_EQ(reader.RemainingBits(), 11);
}

TEST(BitstreamReaderTests, ReadStringNullTerminated) {
    std::vector<uint8_t> buffer = {'H', 'e', 'l', 'l', 'o', '\0'};
    System::IO::BitstreamReader reader(buffer);
    std::string str;
    reader.ReadStringNullTerminated(str, 100);
    EXPECT_EQ(str, "Hello");
}

TEST(BitstreamReaderTests, ReadStringLengthPrefixed8) {
    std::vector<uint8_t> buffer = {5, 'H', 'e', 'l', 'l', 'o'};
    System::IO::BitstreamReader reader(buffer);
    std::string str;
    reader.ReadStringLengthPrefixed8(str, 100);
    EXPECT_EQ(str, "Hello");
}

TEST(BitstreamReaderTests, ReadStringLengthPrefixed32) {
    std::vector<uint8_t> buffer = {5, 0, 0, 0, 'H', 'e', 'l', 'l', 'o'};
    System::IO::BitstreamReader reader(buffer);
    std::string str;
    reader.ReadStringLengthPrefixed32(str, 100);
    EXPECT_EQ(str, "Hello");
}

TEST(BitstreamReaderTests, OutOfBounds) {
    std::vector<uint8_t> buffer = {0, 0};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_THROW(reader.ReadBits(17), std::out_of_range);
}

