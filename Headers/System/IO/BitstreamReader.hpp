#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>

namespace System {
    namespace IO {
        class BitstreamReader {
        public:
            enum class BitOrder { LSB0, MSB0 };
        private:
            const uint8_t* data;
            size_t dataSize;
            size_t bitPos;
            
            BitOrder order;
        public:
            BitstreamReader(const std::vector<uint8_t>& buffer) : data(buffer.data()), dataSize(buffer.size()), bitPos(0), order(BitOrder::LSB0) {}
            BitstreamReader(const uint8_t* buffer, size_t size) : data(buffer), dataSize(size), bitPos(0), order(BitOrder::LSB0) {}

            void SetBitOrder(BitOrder newOrder) {
                order = newOrder;
            }
            uint32_t ReadBits(size_t count) {
                // 1) Validate count and available bits in one shot
                if (count == 0 || count > 32)
                    throw std::invalid_argument("BitstreamReader [ReadBits]: count must be between 1 and 32");
                if (bitPos + count > dataSize * 8)
                    throw std::out_of_range("BitstreamReader [ReadBits]: reading past buffer");

                uint32_t value = 0;
                // 2) Branch on bit-order once, then loop
                if (order == BitOrder::LSB0) {
                    for (size_t i = 0; i < count; ++i) {
                        size_t byteIndex = bitPos >> 3;
                        size_t bitIndex = bitPos & 7;
                        const auto shift = bitIndex;                // LSB0
                        uint8_t  bit = (data[byteIndex] >> shift) & 1;
                        value |= (bit << i);
                        ++bitPos;
                    }
                }else{ // MSB0
                    for (size_t i = 0; i < count; ++i) {
                        size_t byteIndex = bitPos >> 3;
                        size_t bitIndex = bitPos & 7;
                        const auto shift = 7 - bitIndex;            // MSB0
                        uint8_t  bit = (data[byteIndex] >> shift) & 1;
                        value = (value << 1) | bit;
                        ++bitPos;
                    }
                }
                return value;
            }
            uint32_t ReadBits2(int count) {
                if (count <= 0 || count > 32)
                    throw std::invalid_argument("BitstreamReader [ReadBits]: Bit count must be between 1 and 32");

                uint32_t value = 0;
                for (int i = 0; i < count; ++i) {
                    size_t byteIndex = bitPos / 8;
                    size_t bitIndex = bitPos % 8;

                    if (byteIndex >= dataSize)
                        throw std::out_of_range("BitstreamReader [ReadBits]: reading past buffer");

                    const auto shift = (order == BitOrder::LSB0 ? bitIndex : (7 - bitIndex));
                    uint8_t bit = (data[byteIndex] >> shift) & 1;

                    if (order == BitOrder::LSB0) {
                        value |= (bit << i);
                    }
                    else { // MSB0
                        value = (value << 1) | bit;
                    }
                    ++bitPos;
                }
                return value;
            }
            uint32_t ReadUInt32(){
                uint32_t ret = 0;
                size_t byteIndex = bitPos >> 3;
                int bitOffset = bitPos & 7;
                // easy copy if aligned
                if (bitOffset == 0) {
                    ret = uint32_t(data[byteIndex]);
                    ret |= uint32_t(data[byteIndex + 1]) << 8;
                    ret |= uint32_t(data[byteIndex + 2]) << 16;
                    ret |= uint32_t(data[byteIndex + 3]) << 24;
                    bitPos += 32;
                    return ret;
                }else{


                    bitPos += 32;
                    return ret;
                }
                
            }
            bool ReadBool() {
                size_t byteIndex = bitPos / 8;
                size_t bitIndex = bitPos % 8;
                if (byteIndex >= dataSize) {
                    throw std::out_of_range("BitstreamReader [ReadBool]: reading past buffer");
                }
                const auto shift = (order == BitOrder::LSB0 ? bitIndex : (7 - bitIndex));
                bool b = ((data[byteIndex] >> shift) & 1) != 0;
                ++bitPos;
                return b;
            }
            bool ReadBoolUnchecked() {
                size_t byteIndex = bitPos / 8;
                size_t bitIndex = bitPos % 8;
                const auto shift = (order == BitOrder::LSB0 ? bitIndex : (7 - bitIndex));
                uint8_t bit = ((data[byteIndex] >> shift) & 1) != 0;
                ++bitPos;
                return bit;
            }
            uint32_t PeekBits(int count) {
                if (count <= 0 || count > 32)
                    throw std::invalid_argument("BitstreamReader [PeekBits]: count 1–32");

                size_t origPos = bitPos;
                uint32_t v = ReadBits(count);
                bitPos = origPos;
                return v;
            }
            void SkipBits(int count) {
                if (count < 0 || (bitPos + count) >(dataSize * 8)) {
                    throw std::out_of_range("BitstreamReader [SkipBits] beyond EOF");
                }
                bitPos += count;
            }
            void SkipBitsUnchecked(int count) {
                bitPos += count;
            }
            void SkipBytes(int count) {
                if (count < 0) {
                    throw std::invalid_argument("BitstreamReader [SkipBytes]: count must be non-negative");
                }
                size_t bitsToSkip = static_cast<size_t>(count) << 3;
                if (bitPos + bitsToSkip > dataSize * 8) {
                    throw std::out_of_range("BitstreamReader [SkipBytes]: beyond EOF");
                }
                bitPos += bitsToSkip;
            }
            void SkipBytesUnchecked(int count) {
                bitPos += static_cast<size_t>(count) << 3;
            }
            void AlignToByte() {
                bitPos = (bitPos + 7) & ~7;
            }
            void AlignToInt16(){
                bitPos = (bitPos + 15) & ~15;
            }
            void AlignToInt32() {
                bitPos = (bitPos + 31) & ~31;
            }
            void AlignToInt64() {
                bitPos = (bitPos + 63) & ~63;
            }
            void AlignTo(size_t alignBits) {
                size_t mask = alignBits - 1;
                if ((alignBits & mask) != 0) {
                    throw std::invalid_argument("BitstreamReader [AlignTo]: must be power of two");
                }
                size_t newPos = (bitPos + mask) & ~mask;
                bitPos = newPos;
            }
            size_t GetBitPosition() const {
                return bitPos;
            }
            size_t GetBytePosition() const {
                return bitPos >> 3;
            }
            size_t RemainingBits() const {
                size_t maxBits = dataSize * 8;
                return (bitPos < maxBits ? maxBits - bitPos : 0);
            }

            bool IsEOF() const {
                return bitPos >= dataSize * 8;
            }



        };
    }
}
