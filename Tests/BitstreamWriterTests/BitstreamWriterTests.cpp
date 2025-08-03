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

TEST_F(BitstreamWriterTest, WriteBitsEdgeCases) {
    // Test writing 32 bits
    writer.WriteBits(0xFFFFFFFF, 32);
    EXPECT_EQ(reader.ReadBits(32), 0xFFFFFFFF);

    // Test writing 1 bit
    writer.WriteBits(1, 1);
    EXPECT_EQ(reader.ReadBits(1), 1);

    // Test writing to the exact end of the buffer
    writer.SetBitOrder(System::IO::BitstreamWriter::BitOrder::LSB0);
    reader.SetBitOrder(System::IO::BitstreamReader::BitOrder::LSB0);
    std::vector<uint8_t> small_buffer(4);
    System::IO::BitstreamWriter small_writer(small_buffer);
    small_writer.WriteBits(0xFFFFFFFF, 32);
    EXPECT_THROW(small_writer.WriteBits(1, 1), std::out_of_range);

    // Test exceptions for invalid bit count
    EXPECT_THROW(writer.WriteBits(1, 0), std::invalid_argument);
    EXPECT_THROW(writer.WriteBits(1, 33), std::invalid_argument);

    // Test exception for writing past buffer with a valid bit count
    writer.Clear();
    writer.SkipBits(1024 * 8 - 16); // Go near the end
    EXPECT_THROW(writer.WriteBits(1, 17), std::out_of_range);
}

TEST_F(BitstreamWriterTest, AlignmentFunctions) {
    writer.WriteBits(0, 1); // bitPos = 1
    writer.AlignToByte();   // bitPos = 8
    EXPECT_EQ(writer.IsEOF(), false);
    reader.SkipBits(7);
    EXPECT_EQ(reader.ReadBits(1), 0);

    writer.WriteBits(0, 1); // bitPos = 9
    writer.AlignToInt16();  // bitPos = 16
    reader.SkipBits(7);
    EXPECT_EQ(reader.ReadBits(1), 0);

    writer.WriteBits(0, 1); // bitPos = 17
    writer.AlignToInt32();  // bitPos = 32
    reader.SkipBits(15);
    EXPECT_EQ(reader.ReadBits(1), 0);

    writer.WriteBits(0, 1); // bitPos = 33
    writer.AlignToInt64();  // bitPos = 64
    reader.SkipBits(31);
    EXPECT_EQ(reader.ReadBits(1), 0);
}

TEST_F(BitstreamWriterTest, LargeDataIntegrity) {
    const int dataSize = 512;
    std::vector<uint32_t> testData;
    for(int i = 0; i < dataSize / 4; ++i) {
        uint32_t val = static_cast<uint32_t>(rand());
        testData.push_back(val);
        writer.WriteUInt32(val);
    }

    for(const auto& val : testData) {
        EXPECT_EQ(reader.ReadUInt32(), val);
    }
}

TEST_F(BitstreamWriterTest, ClearFunction) {
    writer.WriteUInt32(0xDEADBEEF);
    writer.Clear();

    // Check if buffer is cleared
    for(size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(buffer[i], 0);
    }

    // Check if position is reset
    writer.WriteUInt32(0xABCDEF12);
    EXPECT_EQ(reader.ReadUInt32(), 0xABCDEF12);
}

TEST_F(BitstreamWriterTest, SkipBitsException) {
    EXPECT_THROW(writer.SkipBits(1024 * 8 + 1), std::out_of_range);
    EXPECT_THROW(writer.SkipBits(-1), std::out_of_range);
}
