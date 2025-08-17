#ifndef _SYSTEM_IO_BITSTREAMWRITER_H
#define _SYSTEM_IO_BITSTREAMWRITER_H



#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm> 
namespace System {
    namespace IO {
        /// <summary>
        /// BitstreamWriter that uses least significant bit first.
        /// For example, if a byte is 0b10110010, the bits are processed in this order:
        /// 0, 1, 0, 0, 1, 1, 0, 1
        /// </summary>
        class BitstreamWriter {
        public:
        private:
            uint8_t* data;
            size_t dataSize;
            size_t bitPos;

            inline uint64_t ToZigZagEncode64(int64_t n) {
#ifdef NAIVE    
                return (n < 0) ? static_cast<uint64_t>(-2 * n - 1) : static_cast<uint64_t>(2 * n);
#else
                return static_cast<uint64_t>((n >> 63) ^ (n << 1));
#endif
            }
            inline uint32_t ToZigZagEncode32(int32_t n) {
#ifdef NAIVE    
                return (n < 0) ? static_cast<uint32_t>(-2 * n - 1) : static_cast<uint32_t>(2 * n);
#else
                return static_cast<uint32_t>((n >> 31) ^ (n << 1));
#endif
            }
            inline int32_t ToZigZagDecode32(uint32_t n) {
#ifdef NAIVE
                if ((n & 1) == 1) {
                    return -static_cast<int32_t>(n / 2);
                }else{
                    return static_cast<int32_t>(n / 2);
                }
#else
                return static_cast<int32_t>((n >> 1) ^ -static_cast<int32_t>(n & 1));
#endif
            }

        public:
            BitstreamWriter( std::vector<uint8_t>& buffer) : data(buffer.data()), dataSize(buffer.size()), bitPos(0)  {
            }
            BitstreamWriter( uint8_t* buffer, size_t size) : data(buffer), dataSize(size), bitPos(0) {
            }
            /// <summary>
            /// Clears the data in the buffer and resets the bit position to 0.
            /// </summary>
            inline void Clear(){
                std::fill(data, data + dataSize, 0);
                bitPos = 0;
            }
            /// <summary>
            /// Writes the specified number of bits up to 32 bits to the stream.
            /// </summary>
            /// <param name="value">the value to write</param>
            /// <param name="count">number of bits to write</param>
            void WriteBits32(uint32_t value, size_t count) {
                if (count == 0 || count > 32)
                    throw std::invalid_argument("BitstreamWriter [WriteBits]: count must be between 1 and 32");
                if (bitPos + count > dataSize * 8)
                    throw std::out_of_range("BitstreamWriter [WriteBits]: writing past buffer");

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
            }
#ifdef NAIVE    
            void WriteBits32Unchecked(uint32_t value, size_t count) {
                for (size_t i = 0; i < count; ++i) {
                    size_t byteIndex = bitPos >> 3;
                    size_t bitIndex = bitPos & 7;

                    uint8_t bit = (value >> i) & 1;

                    if (bit) {
                        data[byteIndex] |= (1 << bitIndex);
                    }else {
                        data[byteIndex] &= ~(1 << bitIndex);
                    }
                    ++bitPos;
                }
            }
#else
            void WriteBits32Unchecked(uint32_t value, size_t count) {
                size_t byteIndex = bitPos >> 3;
                size_t bitOffset = bitPos & 7;

                // Combine value with existing bits in the first byte
                if (bitOffset != 0 || count < 8) {
                    size_t bitsInFirstByte = std::min(count, 8 - bitOffset);
                    uint8_t mask = ((1u << bitsInFirstByte) - 1) << bitOffset;
                    data[byteIndex] = (data[byteIndex] & ~mask) | ((value << bitOffset) & mask);
                    bitPos += bitsInFirstByte;
                    value >>= bitsInFirstByte;
                    count -= bitsInFirstByte;
                    ++byteIndex;
                }

                // Write full bytes
                while (count >= 8) {
                    data[byteIndex++] = static_cast<uint8_t>(value);
                    value >>= 8;
                    bitPos += 8;
                    count -= 8;
                }

                // Write remaining bits
                if (count > 0) {
                    uint8_t mask = (1u << count) - 1;
                    data[byteIndex] = (data[byteIndex] & ~mask) | (value & mask);
                    bitPos += count;
                }
            }
#endif


            void WriteUInt32(uint32_t value, size_t count){
                WriteBits32(value,count);
            }
            void WriteUInt32(uint32_t value) {
                const size_t maxBits = dataSize * 8;
                if (bitPos + 32 > maxBits) [[unlikely]] {
                    throw std::out_of_range("BitstreamWriter [WriteUInt32]: writing past buffer");
                }

                size_t byteIndex = bitPos >> 3;
                size_t bitOffset = bitPos & 7;

                if (bitOffset == 0) { // Aligned write
                    data[byteIndex] = value & 0xFF;
                    data[byteIndex + 1] = (value >> 8) & 0xFF;
                    data[byteIndex + 2] = (value >> 16) & 0xFF;
                    data[byteIndex + 3] = (value >> 24) & 0xFF;
                    bitPos += 32;
                }else{ // Unaligned write
                    for (int i = 0; i < 32; ++i) {
                        WriteBool((value >> i) & 1);
                    }
                }
            }
            void WriteBool(bool value) {
                size_t byteIndex = bitPos / 8;
                size_t bitIndex = bitPos % 8;
                if (byteIndex >= dataSize) [[unlikely]] {
                    throw std::out_of_range("BitstreamWriter [WriteBool]: writing past buffer");
                }
                const auto shift = bitIndex;
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
                const auto shift =  bitIndex;
                if (value) {
                    data[byteIndex] |= (1 << shift);
                }else {
                    data[byteIndex] &= ~(1 << shift);
                }
                ++bitPos;
            }
            inline bool IsEOF() const {
                return bitPos >= dataSize * 8;
            }

            inline void SkipBits(int count) {
                if (count < 0 || (bitPos + count) > (dataSize * 8)) [[unlikely]] {
                    throw std::out_of_range("BitstreamWriter [SkipBits] beyond EOF");
                }
                bitPos += count;
            }
            inline void SkipBitsUnchecked(int count) {
                bitPos += count;
            }
            inline void SkipBytesUnchecked(int count) {
                bitPos += static_cast<size_t>(count) << 3;
            }
            inline void AlignToBits(uint32_t alignment) {
                size_t mask = static_cast<size_t>(alignment - 1);
                bitPos = (bitPos + mask) & ~mask;
            }
            inline void AlignToByte() {
                bitPos = (bitPos + 7) & ~7;
            }
            inline void AlignToInt16() {
                bitPos = (bitPos + 15) & ~15;
            }
            inline void AlignToInt32() {
                bitPos = (bitPos + 31) & ~31;
            }
            inline void AlignToInt64() {
                bitPos = (bitPos + 63) & ~63;
            }
            void Write7BitEncodedInt(int32_t value) {
                AlignToByte();
                uint32_t val = (uint32_t)value;
                while (val >= 0x80) {
                    WriteUInt8((uint8_t)(val | 0x80));
                    val >>= 7;
                }
                WriteUInt8((uint8_t)val);
            }

            void Write7BitEncodedInt64(int64_t value) {
                AlignToByte();
                uint64_t val = (uint64_t)value;
                while (val >= 0x80) {
                    WriteUInt8((uint8_t)(val | 0x80));
                    val >>= 7;
                }
                WriteUInt8((uint8_t)val);
            }
            void WriteStringLengthPrefixed8(const std::string& str) {
                if (str.length() > 255) [[unlikely]] {
                    throw std::length_error("BitstreamWriter [WriteStringLengthPrefixed8]: string length exceeds 255");
                }
                AlignToByte();
                WriteUInt8(str.length());
                for (char c : str) {
                    WriteUInt8(c);
                }
            }
            void WriteStringLengthPrefixed32(const std::string& str) {
                AlignToByte();
                WriteUInt32(str.length());
                for (char c : str) {
                    WriteUInt8(c);
                }
            }
            void WriteUInt8(uint8_t value) {
                const size_t maxBits = dataSize * 8;
                if (bitPos + 8 > maxBits) [[unlikely]] {
                    throw std::out_of_range("BitstreamWriter [WriteUInt8]: writing past buffer");
                }

                size_t byteIndex = bitPos >> 3;
                size_t bitOffset = bitPos & 7;

                if (bitOffset == 0) { // Aligned write
                    data[byteIndex] = value;
                    bitPos += 8;
                }else{ // Unaligned write
                    for (int i = 0; i < 8; ++i) {
                        WriteBool((value >> i) & 1);
                    }
                }
            }



        };
    }
}

#endif