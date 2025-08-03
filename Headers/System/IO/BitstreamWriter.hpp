#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>

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
            BitstreamWriter(const std::vector<uint8_t>& buffer) : data(buffer.data()), dataSize(buffer.size()), bitPos(0), order(BitOrder::LSB0) {}
            BitstreamWriter(const uint8_t* buffer, size_t size) : data(buffer), dataSize(size), bitPos(0), order(BitOrder::LSB0) {}

            void SetBitOrder(BitOrder newOrder) {
                order = newOrder;
            }
            void WriteUInt32(uint32_t value) {
                size_t byteIndex = bitPos >> 3;
                size_t bitOffset = bitPos & 7;
                if (bitOffset == 0) {

                }else{
                
                }
            }
            void WriteBool(bool value) {
                size_t byteIndex = bitPos / 8;
                size_t bitIndex = bitPos % 8;
                if (byteIndex >= dataSize) {
                    throw std::out_of_range("BitstreamReader [ReadBool]: reading past buffer");
                }
                if (value) {
                    const auto shift = (order == BitOrder::LSB0 ? bitIndex : (7 - bitIndex));
                    data[byteIndex] = 1 << shift;
                }
                ++bitPos;
            }


            bool IsEOF() const {
                return bitPos >= dataSize * 8;
            }

        };
    }
}