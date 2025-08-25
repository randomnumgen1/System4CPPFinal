#include <gtest/gtest.h>
#include "System/IO/BitstreamReader.hpp"
#include <vector>

TEST(BitstreamReaderTests, ReadBitsSimple) {
    std::vector<uint8_t> buffer = {0b11010101}; // 213
    System::IO::BitstreamReader reader(buffer);
	
	
	
    // Reads 4 bits: 0101 -> 5
    EXPECT_EQ(reader.ReadBits32(4), 5);
    EXPECT_EQ(reader.GetBitPosition(), 4);

    // Reads next 4 bits: 1101 -> 13
    EXPECT_EQ(reader.ReadBits32(4), 13);
    EXPECT_EQ(reader.GetBitPosition(), 8);
    ASSERT_TRUE(reader.IsEOF());
}

TEST(BitstreamReaderTests, ReadAcrossByteBoundary) {
    std::vector<uint8_t> buffer = {0b10101010, 0b11001100};
    System::IO::BitstreamReader reader(buffer);

    reader.SkipBits(4); // Move to the middle of the first byte
    // Reads 8 bits, 4 from first byte (1010) and 4 from second (1100)
    // Expected: 11001010 -> 202
    EXPECT_EQ(reader.ReadBits32(8), 202);
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
    std::vector<uint8_t> buffer = {0xAA, 0xCC, 0xF0}; // 10101010 11001100 11110000
    System::IO::BitstreamReader reader(buffer);
    reader.SkipBits(4); // bitPos = 4
    // Reads 16 bits starting from bit 4
    // The bits are (from LSB to MSB):
    // from byte 0 (last 4 bits): 1010
    // from byte 1 (all 8 bits): 11001100
    // from byte 2 (first 4 bits): 1111
    // The code reads them as a little-endian integer from the byte stream and shifts.
    // The value should be (0xF0CCAA >> 4) & 0xFFFF = 0x0CCA
    EXPECT_EQ(reader.ReadUInt16(), 0x0CCA);
    EXPECT_EQ(reader.GetBitPosition(), 20);
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

TEST(BitstreamReaderTests, ReadUInt32_Unaligned) {
    std::vector<uint8_t> buffer = {0xAA, 0xCC, 0xF0, 0x33, 0x55};
    System::IO::BitstreamReader reader(buffer);
    reader.SkipBits(4);
    // The value should be (0x5533F0CCAA >> 4) & 0xFFFFFFFF = 0x533F0CCA
    EXPECT_EQ(reader.ReadUInt32(), 0x533F0CCA);
    EXPECT_EQ(reader.GetBitPosition(), 36);
}

TEST(BitstreamReaderTests, ReadInt32_Unaligned) {
    std::vector<uint8_t> buffer = {0x11, 0x22, 0x33, 0x44, 0x55};
    System::IO::BitstreamReader reader(buffer);
    reader.SkipBits(4);
    // The value should be (0x5544332211 >> 4) & 0xFFFFFFFF = 0x54433221
    EXPECT_EQ(reader.ReadUInt32(), 0x54433221);
    EXPECT_EQ(reader.GetBitPosition(), 36);
}

TEST(BitstreamReaderTests, ReadAcrossMultipleByteBoundaries) {
    std::vector<uint8_t> buffer = {0xAA, 0xCC, 0xF0, 0x33};
    System::IO::BitstreamReader reader(buffer);
    reader.SkipBits(3);
    // This will read 5 bits from byte 0, 8 from byte 1, 8 from byte 2, and 4 from byte 3.
    // The expected value is 8264085 (0x7E1995)
    EXPECT_EQ(reader.ReadBits32(25), 8264085);
    EXPECT_EQ(reader.GetBitPosition(), 28);
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
    EXPECT_EQ(reader.ReadBits32(4), 5);
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
TEST(BitstreamReaderTests, RemainingBits_EdgeCases) {
    // Test with an empty buffer
    std::vector<uint8_t> empty_buffer;
    System::IO::BitstreamReader empty_reader(empty_buffer);
    EXPECT_EQ(empty_reader.RemainingBits(), 0);

    // Test with a buffer where all bits have been read
    std::vector<uint8_t> buffer = {0, 0};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.RemainingBits(), 16);
    reader.ReadBits32(16);
    EXPECT_EQ(reader.RemainingBits(), 0);

    // Test with a buffer, read some bits, then all bits
    std::vector<uint8_t> buffer2 = {0, 0, 0};
    System::IO::BitstreamReader reader2(buffer2);
    EXPECT_EQ(reader2.RemainingBits(), 24);
    reader2.ReadBits32(10);
    EXPECT_EQ(reader2.RemainingBits(), 14);
    reader2.ReadBits32(14);
    EXPECT_EQ(reader2.RemainingBits(), 0);
}
TEST(BitstreamReaderTests, PeekSkipEdgeCases) {
    std::vector<uint8_t> buffer = {0xAA, 0xCC, 0xF0, 0x33};
    System::IO::BitstreamReader reader(buffer);

    // Peek across byte boundary
    reader.SkipBits(4);
    EXPECT_EQ(reader.PeekBits(16), 0x0CCA);
    EXPECT_EQ(reader.GetBitPosition(), 4); // Position should not change

    // Skip to end
    System::IO::BitstreamReader reader2(buffer);
    reader2.SkipBits(32);
    EXPECT_TRUE(reader2.IsEOF());

    // Skip past end
    System::IO::BitstreamReader reader3(buffer);
    EXPECT_THROW(reader3.SkipBits(33), std::out_of_range);

    // Peek past end
    System::IO::BitstreamReader reader4(buffer);
    reader4.SkipBits(20);
    EXPECT_THROW(reader4.PeekBits(13), std::out_of_range);
}

TEST(BitstreamReaderTests, AlignToByte) {
    std::vector<uint8_t> buffer = {0, 0, 0, 0};
    System::IO::BitstreamReader reader(buffer);
    reader.ReadBits32(3);
    reader.AlignToByte();
    EXPECT_EQ(reader.GetBitPosition(), 8);
}

TEST(BitstreamReaderTests, AlignFunctions) {
    std::vector<uint8_t> buffer(10, 0); // 80 bits
    System::IO::BitstreamReader reader(buffer);

    reader.SkipBits(3);
    reader.AlignToByte();
    EXPECT_EQ(reader.GetBitPosition(), 8);

    reader.SkipBits(1); // bit pos is 9
    reader.AlignToInt16();
    EXPECT_EQ(reader.GetBitPosition(), 16);

    reader.SkipBits(1); // bit pos is 17
    reader.AlignToInt32();
    EXPECT_EQ(reader.GetBitPosition(), 32);

    reader.SkipBits(1); // bit pos is 33
    reader.AlignToInt64();
    EXPECT_EQ(reader.GetBitPosition(), 64);

    System::IO::BitstreamReader reader2(buffer);
    reader2.SkipBits(3);
    reader2.AlignTo(4);
    EXPECT_EQ(reader2.GetBitPosition(), 4);

    reader2.SkipBits(1); // bit pos is 5
    reader2.AlignTo(16);
    EXPECT_EQ(reader2.GetBitPosition(), 16);

    EXPECT_THROW(reader2.AlignTo(3), std::invalid_argument);
}

TEST(BitstreamReaderTests, IsEOF) {
    std::vector<uint8_t> buffer = {0, 0};
    System::IO::BitstreamReader reader(buffer);
    reader.ReadBits32(15);
    EXPECT_FALSE(reader.IsEOF());
    reader.ReadBits32(1);
    EXPECT_TRUE(reader.IsEOF());
}

TEST(BitstreamReaderTests, RemainingBits) {
    std::vector<uint8_t> buffer = {0, 0};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_EQ(reader.RemainingBits(), 16);
    reader.ReadBits32(5);
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

TEST(BitstreamReaderTests, StringEdgeCases) {
    // Null-terminated string edge cases
    std::vector<uint8_t> buffer1 = {'\0', 'H', 'e', 'l', 'l', 'o'};
    System::IO::BitstreamReader reader1(buffer1);
    std::string str1;
    reader1.ReadStringNullTerminated(str1, 100);
    EXPECT_TRUE(str1.empty());

    std::vector<uint8_t> buffer2 = {'H', 'e', 'l', 'l', 'o'};
    System::IO::BitstreamReader reader2(buffer2);
    std::string str2;
    reader2.ReadStringNullTerminated(str2, 5);
    EXPECT_EQ(str2, "Hello");

    std::vector<uint8_t> buffer3;
    System::IO::BitstreamReader reader3(buffer3);
    std::string str3;
    reader3.ReadStringNullTerminated(str3, 100);
    EXPECT_TRUE(str3.empty());

    // Length-prefixed string edge cases
    std::vector<uint8_t> buffer4 = {0, 'H', 'e', 'l', 'l', 'o'};
    System::IO::BitstreamReader reader4(buffer4);
    std::string str4;
    reader4.ReadStringLengthPrefixed8(str4, 100);
    EXPECT_TRUE(str4.empty());

    std::vector<uint8_t> buffer5 = {10, 'H', 'e', 'l', 'l', 'o'}; // Prefix > actual
    System::IO::BitstreamReader reader5(buffer5);
    std::string str5;
    EXPECT_THROW(reader5.ReadStringLengthPrefixed8(str5, 100), std::out_of_range);
    
    std::vector<uint8_t> buffer6 = {5, 0, 0, 0, 'H', 'e', 'l', 'l', 'o'};
    System::IO::BitstreamReader reader6(buffer6);
    std::string str6;
    EXPECT_THROW(reader6.ReadStringLengthPrefixed32(str6, 4), std::runtime_error);
}

TEST(BitstreamReaderTests, OutOfBounds) {
    std::vector<uint8_t> buffer = {0, 0};
    System::IO::BitstreamReader reader(buffer);
    EXPECT_THROW(reader.ReadBits32(17), std::out_of_range);
}


TEST(BitstreamReaderTests, Read7BitEncodedUInt32) {
    // Test case 1: Single-byte value
    std::vector<uint8_t> buffer1 = {100};
    System::IO::BitstreamReader reader1(buffer1);
    EXPECT_EQ(reader1.Read7BitEncodedUInt32(), 100);
    EXPECT_EQ(reader1.GetBitPosition(), 8);

    // Test case 2: Multi-byte value (300)
    std::vector<uint8_t> buffer2 = {0xAC, 0x02};
    System::IO::BitstreamReader reader2(buffer2);
    EXPECT_EQ(reader2.Read7BitEncodedUInt32(), 300);
    EXPECT_EQ(reader2.GetBitPosition(), 16);

    // Test case 3: Maximum value (UINT32_MAX)
    std::vector<uint8_t> buffer3 = {0xFF, 0xFF, 0xFF, 0xFF, 0x0F};
    System::IO::BitstreamReader reader3(buffer3);
    EXPECT_EQ(reader3.Read7BitEncodedUInt32(), UINT32_MAX);
    EXPECT_EQ(reader3.GetBitPosition(), 40);

    // Test case 4: Unaligned stream
    std::vector<uint8_t> buffer4 = {0x00, 0xAC, 0x02}; // Some padding byte
    System::IO::BitstreamReader reader4(buffer4);
    reader4.ReadBits32(4); // Read 4 bits to unalign the stream
    EXPECT_EQ(reader4.Read7BitEncodedUInt32(), 300);
    // Position should be 8 (for alignment) + 16 (for the read) = 24
    EXPECT_EQ(reader4.GetBitPosition(), 24);

    // Test case 5: Oversized value
    std::vector<uint8_t> buffer5 = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01}; // Too large
    System::IO::BitstreamReader reader5(buffer5);
    EXPECT_THROW(reader5.Read7BitEncodedUInt32(), std::runtime_error);

    // Test case 6: Another oversized value (5th byte > 0x0F)
    std::vector<uint8_t> buffer6 = {0xFF, 0xFF, 0xFF, 0xFF, 0x10};
    System::IO::BitstreamReader reader6(buffer6);
    EXPECT_THROW(reader6.Read7BitEncodedUInt32(), std::runtime_error);
}

TEST(BitstreamReaderTests, Read7BitEncodedUInt64) {
    // Test case 1: Single-byte value
    std::vector<uint8_t> buffer1 = {120};
    System::IO::BitstreamReader reader1(buffer1);
    EXPECT_EQ(reader1.Read7BitEncodedUInt64(), 120);
    EXPECT_EQ(reader1.GetBitPosition(), 8);

    // Test case 2: Multi-byte value (100000)
    std::vector<uint8_t> buffer2 = {0xA0, 0x8D, 0x06};
    System::IO::BitstreamReader reader2(buffer2);
    EXPECT_EQ(reader2.Read7BitEncodedUInt64(), 100000);
    EXPECT_EQ(reader2.GetBitPosition(), 24);

    // Test case 3: Maximum value (UINT64_MAX)
    std::vector<uint8_t> buffer3 = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01};
    System::IO::BitstreamReader reader3(buffer3);
    EXPECT_EQ(reader3.Read7BitEncodedUInt64(), UINT64_MAX);
    EXPECT_EQ(reader3.GetBitPosition(), 80);

    // Test case 4: Oversized value
    std::vector<uint8_t> buffer4 = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0x01}; // Too large
    System::IO::BitstreamReader reader4(buffer4);
    EXPECT_THROW(reader4.Read7BitEncodedUInt64(), std::runtime_error);

    // Test case 5: Another oversized value (10th byte > 0x01)
    std::vector<uint8_t> buffer5 = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x02};
    System::IO::BitstreamReader reader5(buffer5);
    EXPECT_THROW(reader5.Read7BitEncodedUInt64(), std::runtime_error);
}
