#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm> 
namespace System {
    namespace IO {
        class BitstreamWriter {
        public:
            enum class BitOrder { LSB0, MSB0 };
        private:
            uint8_t* data;
            size_t dataSize;
            size_t bitPos;

            BitOrder order;
        public:
            BitstreamWriter( std::vector<uint8_t>& buffer) : data(buffer.data()), dataSize(buffer.size()), bitPos(0), order(BitOrder::LSB0) {
            }
            BitstreamWriter( uint8_t* buffer, size_t size) : data(buffer), dataSize(size), bitPos(0), order(BitOrder::LSB0) {
            }

            void SetBitOrder(BitOrder newOrder) {
                order = newOrder;
            }
            void Clear(){
                std::fill(data, data + dataSize, 0);
                bitPos = 0;
            }
            void WriteBits(uint32_t value, size_t count) {
                if (count == 0 || count > 32)
                    throw std::invalid_argument("BitstreamWriter [WriteBits]: count must be between 1 and 32");
                if (bitPos + count > dataSize * 8)
                    throw std::out_of_range("BitstreamWriter [WriteBits]: writing past buffer");

                if (order == BitOrder::LSB0) {
                    for (size_t i = 0; i < count; ++i) {
                        size_t byteIndex = bitPos >> 3;
                        size_t bitIndex = bitPos & 7;

                        uint8_t bit = (value >> i) & 1;

                        if (bit) {
                            data[byteIndex] |= (1 << bitIndex);
                        }
                        else {
                            data[byteIndex] &= ~(1 << bitIndex);
                        }

                        ++bitPos;
                    }
                }else{ // MSB0
                    for (size_t i = 0; i < count; ++i) {
                        size_t byteIndex = bitPos >> 3;
                        size_t bitIndex = bitPos & 7;

                        uint8_t bit = (value >> (count - 1 - i)) & 1;

                        if (bit) {
                            data[byteIndex] |= (1 << (7 - bitIndex));
                        }
                        else {
                            data[byteIndex] &= ~(1 << (7 - bitIndex));
                        }

                        ++bitPos;
                    }
                }
            }
            void WriteUInt32(uint32_t value) {
                const size_t maxBits = dataSize * 8;
                if (bitPos + 32 > maxBits) {
                    throw std::out_of_range("BitstreamWriter [WriteUInt32]: writing past buffer");
                }

                size_t byteIndex = bitPos >> 3;
                size_t bitOffset = bitPos & 7;

                if (bitOffset == 0) { // Aligned write
                    if (order == BitOrder::LSB0) {
                        data[byteIndex] = value & 0xFF;
                        data[byteIndex + 1] = (value >> 8) & 0xFF;
                        data[byteIndex + 2] = (value >> 16) & 0xFF;
                        data[byteIndex + 3] = (value >> 24) & 0xFF;
                    }else{ // MSB0
                        data[byteIndex] = (value >> 24) & 0xFF;
                        data[byteIndex + 1] = (value >> 16) & 0xFF;
                        data[byteIndex + 2] = (value >> 8) & 0xFF;
                        data[byteIndex + 3] = value & 0xFF;
                    }
                    bitPos += 32;
                }else{ // Unaligned write
                    if (order == BitOrder::LSB0) {
                        for (int i = 0; i < 32; ++i) {
                            WriteBool((value >> i) & 1);
                        }
                    }else{ // MSB0
                        for (int i = 0; i < 32; ++i) {
                            WriteBool((value >> (31 - i)) & 1);
                        }
                    }
                }
            }
            void WriteBool(bool value) {
                size_t byteIndex = bitPos / 8;
                size_t bitIndex = bitPos % 8;
                if (byteIndex >= dataSize) {
                    throw std::out_of_range("BitstreamWriter [WriteBool]: writing past buffer");
                }
                const auto shift = (order == BitOrder::LSB0 ? bitIndex : (7 - bitIndex));
                if (value) {
                    data[byteIndex] |= 1 << shift;
                }else{
                    data[byteIndex] &= ~(1 << shift);
                }
                ++bitPos;
            }
            void WriteBoolUnchecked(bool value) {
                size_t byteIndex = bitPos >> 3;
                size_t bitIndex = bitPos & 7;
                const auto shift = (order == BitOrder::LSB0 ? bitIndex : (7 - bitIndex));
                if (value) {
                    data[byteIndex] |= (1 << shift);
                }else {
                    data[byteIndex] &= ~(1 << shift);
                }
                ++bitPos;
            }
            bool IsEOF() const {
                return bitPos >= dataSize * 8;
            }

            void SkipBits(int count) {
                if (count < 0 || (bitPos + count) > (dataSize * 8)) {
                    throw std::out_of_range("BitstreamWriter [SkipBits] beyond EOF");
                }
                bitPos += count;
            }
            void SkipBitsUnchecked(int count) {
                bitPos += count;
            }
            void SkipBytesUnchecked(int count) {
                bitPos += static_cast<size_t>(count) << 3;
            }
            void AlignToByte() {
                bitPos = (bitPos + 7) & ~7;
            }
            void AlignToInt16() {
                bitPos = (bitPos + 15) & ~15;
            }
            void AlignToInt32() {
                bitPos = (bitPos + 31) & ~31;
            }
            void AlignToInt64() {
                bitPos = (bitPos + 63) & ~63;
            }







        };
    }
}