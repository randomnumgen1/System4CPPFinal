#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>

namespace System {
    namespace IO {
        class BitstreamReader {
        private:
            const uint8_t* data;
            size_t dataSize;
            size_t bitPos;
        public:
            BitstreamReader(const std::vector<uint8_t>& buffer) : data(buffer.data()), dataSize(buffer.size()), bitPos(0) {}
            BitstreamReader(const uint8_t* buffer, size_t size) : data(buffer), dataSize(size), bitPos(0) {}


            uint32_t ReadBits(int count) {
                if (count <= 0 || count > 32)
                    throw std::invalid_argument("Bit count must be between 1 and 32");

                uint32_t value = 0;
                for (int i = 0; i < count; ++i) {
                    size_t byteIndex = bitPos / 8;
                    size_t bitIndex = bitPos % 8;

                    if (byteIndex >= dataSize)
                        throw std::out_of_range("BitstreamReader: reading past buffer");

                    uint8_t bit = (data[byteIndex] >> bitIndex) & 1;
                    value |= (bit << i);
                    ++bitPos;
                }
                return value;
            }

            bool ReadBit() {
                return ReadBits(1);
            }

            void AlignToByte() {
                bitPos = ((bitPos + 7) / 8) * 8;
            }

            size_t GetBitPosition() const {
                return bitPos;
            }

            size_t GetBytePosition() const {
                return bitPos / 8;
            }

            bool IsEOF() const {
                return bitPos >= dataSize * 8;
            }



        };
    }
}
