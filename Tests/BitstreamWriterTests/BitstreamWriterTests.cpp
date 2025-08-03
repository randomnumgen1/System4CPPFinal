#include <gtest/gtest.h>
#include "System/IO/BitstreamWriter.hpp"
#include "System/IO/BitstreamReader.hpp"
#include <vector>
#include <cstdint>

// Test fixture for Bitstream tests
class BitstreamWriterTest : public ::testing::Test {
protected:
    std::vector<uint8_t> buffer;
    System::IO::BitstreamWriter writer;
    System::IO::BitstreamReader reader;

    BitstreamWriterTest() : buffer(1024), writer(buffer), reader(buffer) {}

    void SetUp() override {
        // Reset buffer and streams before each test
        std::fill(buffer.begin(), buffer.end(), 0);
        writer = System::IO::BitstreamWriter(buffer);
        reader = System::IO::BitstreamReader(buffer);
    }
};

TEST_F(BitstreamWriterTest, WriteReadBitsSimpleLSB0) {
    writer.SetBitOrder(System::IO::BitstreamWriter::BitOrder::LSB0);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    writer.WriteBits(0b1011, 4);
    writer.WriteBits(0b1101, 4);

    EXPECT_EQ(reader.ReadBits(4), 0b1011);
    EXPECT_EQ(reader.ReadBits(4), 0b1101);
}

TEST_F(BitstreamWriterTest, WriteReadBitsSimpleMSB0) {
    writer.SetBitOrder(System::IO::BitstreamWriter::BitOrder::MSB0);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::MSB0);

    writer.WriteBits(0b1011, 4);
    writer.WriteBits(0b1101, 4);

    EXPECT_EQ(reader.ReadBits(4), 0b1011);
    EXPECT_EQ(reader.ReadBits(4), 0b1101);
}

TEST_F(BitstreamWriterTest, WriteAcrossByteBoundaryLSB0) {
    writer.SetBitOrder(System::IO::BitstreamWriter::BitOrder::LSB0);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    writer.WriteBits(0xAB, 8);
    writer.WriteBits(0xCD, 8);

    reader.SkipBits(4);
    EXPECT_EQ(reader.ReadBits(8), 218); // 0xDA
}

TEST_F(BitstreamWriterTest, WriteAcrossByteBoundaryMSB0) {
    writer.SetBitOrder(System::IO::BitstreamWriter::BitOrder::MSB0);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::MSB0);

    writer.WriteBits(0xAB, 8);
    writer.WriteBits(0xCD, 8);

    reader.SkipBits(4);
    EXPECT_EQ(reader.ReadBits(8), 188); // 0xBC
}

TEST_F(BitstreamWriterTest, WriteUInt32AlignedLSB0) {
    writer.SetBitOrder(System::IO::BitstreamWriter::BitOrder::LSB0);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    uint32_t value = 0xDEADBEEF;
    writer.WriteUInt32(value);
    EXPECT_EQ(reader.ReadUInt32(), value);
}

TEST_F(BitstreamWriterTest, WriteUInt32AlignedMSB0) {
    writer.SetBitOrder(System::IO::BitstreamWriter::BitOrder::MSB0);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::MSB0);

    uint32_t value = 0xDEADBEEF;
    writer.WriteUInt32(value);
    EXPECT_EQ(reader.ReadUInt32(), value);
}

TEST_F(BitstreamWriterTest, WriteUInt32UnalignedLSB0) {
    writer.SetBitOrder(System::IO::BitstreamWriter::BitOrder::LSB0);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    writer.WriteBits(0xF, 4); // Misalign by 4 bits

    uint32_t value = 0xDEADBEEF;
    writer.WriteUInt32(value);

    reader.SkipBits(4);
    EXPECT_EQ(reader.ReadUInt32(), value);
}

TEST_F(BitstreamWriterTest, WriteUInt32UnalignedMSB0) {
    writer.SetBitOrder(System::IO::BitstreamWriter::BitOrder::MSB0);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::MSB0);

    writer.WriteBits(0xF, 4); // Misalign by 4 bits

    uint32_t value = 0xDEADBEEF;
    writer.WriteUInt32(value);

    reader.SkipBits(4);
    EXPECT_EQ(reader.ReadUInt32(), value);
}

TEST_F(BitstreamWriterTest, WriteBool) {
    writer.WriteBool(true);
    writer.WriteBool(false);
    writer.WriteBool(true);
    writer.WriteBool(true);

    EXPECT_TRUE(reader.ReadBool());
    EXPECT_FALSE(reader.ReadBool());
    EXPECT_TRUE(reader.ReadBool());
    EXPECT_TRUE(reader.ReadBool());
}

TEST_F(BitstreamWriterTest, MixedWrites) {
    writer.SetBitOrder(System::IO::BitstreamWriter::BitOrder::LSB0);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);

    writer.WriteBits(0x5, 3); // 101
    writer.WriteBool(true);
    writer.WriteUInt32(0x12345678);
    writer.WriteBits(0xA, 4); // 1010

    EXPECT_EQ(reader.ReadBits(3), 0x5);
    EXPECT_TRUE(reader.ReadBool());
    EXPECT_EQ(reader.ReadUInt32(), 0x12345678);
    EXPECT_EQ(reader.ReadBits(4), 0xA);
}
