#ifndef _SYSTEM_IO_BITSTREAMREADER_H
#define _SYSTEM_IO_BITSTREAMREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>

namespace System {
    namespace IO {
        /// <summary>
        /// BitstreamReader that uses least significant bit first.
        /// For example, if a byte is 0b10110010, the bits are processed in this order:
        /// 0, 1, 0, 0, 1, 1, 0, 1
        /// </summary>
        class BitstreamReader {
        public:
        private:
            
            size_t dataSizeInBytes;
            size_t bitPos;
            

            uint8_t ReverseBits(uint8_t b) {
                b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
                b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
                b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
                return b;
            }
        public:
            const uint8_t* data;
            BitstreamReader(const std::vector<uint8_t>& buffer) : data(buffer.data()), dataSizeInBytes(buffer.size()), bitPos(0) {}
            /// <summary>
            /// Initializes a new instance of the <c>BitstreamReader</c> with the given buffer.
            /// </summary>
            /// <param name="buffer">Pointer to the bitstream data.</param>
            /// <param name="size">Size of the buffer in bytes.</param>
            BitstreamReader(const uint8_t* buffer, size_t size) : data(buffer), dataSizeInBytes(size), bitPos(0)  { assert(buffer != nullptr && size > 0); }
            /// <summary>
            /// Reads the specified number of bits up to 32 bits from the stream.
            /// </summary>
            /// <param name="count">number of bits to read</param>
            /// <returns></returns>
            uint32_t ReadBits32(size_t count) {
                if (count == 0 || count > 32)
                    throw std::invalid_argument("BitstreamReader [ReadBits]: count must be between 1 and 32");
                if (bitPos + count > dataSizeInBytes * 8)
                    throw std::out_of_range("BitstreamReader [ReadBits]: reading past buffer");
                return ReadBits32Unchecked(count);
            }
            uint32_t ReadBits32Unchecked(size_t count) {
                uint32_t value = 0;
                // 2) Branch on bit-order once, then loop
                for (size_t i = 0; i < count; ++i) {
                    size_t byteIndex = bitPos >> 3;
                    size_t bitIndex = bitPos & 7;
                    const auto shift = bitIndex;                // LSB0
                    uint8_t  bit = (data[byteIndex] >> shift) & 1;
                    value |= (bit << i);
                    ++bitPos;
                }
                return value;
            }
            uint32_t ReadBits32Unchecked2(uint8_t count) {
                size_t byteOffset = bitPos / 8;
                uint8_t bitOffset = bitPos % 8;
                //the number of bytes that will be effected by the read operation.
                int numbytesaffected = (bitOffset + count + 7) / 8;




                uint32_t lo =
                    (static_cast<uint32_t>(data[byteOffset])) |
                    (static_cast<uint32_t>(data[byteOffset + 1]) << 8) |
                    (static_cast<uint32_t>(data[byteOffset + 2]) << 16) |
                    (static_cast<uint32_t>(data[byteOffset + 3]) << 24);

                uint32_t hi = static_cast<uint32_t>(data[byteOffset + 4]);

                uint32_t combined = (hi << 24) | lo;
                // Create a mask to isolate 'n' bits.
                uint32_t mask = ~0u >> ((32 - count) & 31);

                uint32_t result = (combined >> bitOffset) & mask;

                bitPos += count;
                return result;
            }
            uint32_t ReadBits32Unchecked3(size_t count) {
                size_t byteIndex = bitPos >> 3;
                uint8_t bitOffset = bitPos & 7;

                // Safely copy required bytes into a local buffer to avoid out-of-bounds reads,
                // then use the blending technique for a fast, branchless read.
                uint8_t buffer[5] = { 0 };
                int bytesToRead = (bitOffset + count + 7) / 8;
                for (int i = 0; i < bytesToRead && i < 5; ++i) {
                    if (byteIndex + i < dataSizeInBytes) {
                        buffer[i] = data[byteIndex + i];
                    }
                }

                uint32_t lo =
                    static_cast<uint32_t>(buffer[0]) |
                    (static_cast<uint32_t>(buffer[1]) << 8) |
                    (static_cast<uint32_t>(buffer[2]) << 16) |
                    (static_cast<uint32_t>(buffer[3]) << 24);

                uint32_t hi =
                    static_cast<uint32_t>(buffer[1]) |
                    (static_cast<uint32_t>(buffer[2]) << 8) |
                    (static_cast<uint32_t>(buffer[3]) << 16) |
                    (static_cast<uint32_t>(buffer[4]) << 24);

                uint32_t result = (lo >> bitOffset) | (hi << (32 - bitOffset));

                uint32_t mask = (count == 32) ? ~0u : ((1u << count) - 1);
                result &= mask;

                bitPos += count;
                return result;
            }
            uint32_t ReadBits32Unchecked4(size_t count) {
                size_t byteIndex = bitPos >> 3;
                uint8_t bitOffset = bitPos & 7;

                uint8_t buffer[5] = { 0 };
                int bytesToRead = (bitOffset + count + 7) / 8;
                for (int i = 0; i < bytesToRead && i < 5; ++i) {
                    if (byteIndex + i < dataSizeInBytes) {
                        buffer[i] = data[byteIndex + i];
                    }
                }

                uint32_t lo = static_cast<uint32_t>(buffer[0]) |
                    (static_cast<uint32_t>(buffer[1]) << 8) |
                    (static_cast<uint32_t>(buffer[2]) << 16) |
                    (static_cast<uint32_t>(buffer[3]) << 24);

                uint32_t hi = static_cast<uint32_t>(buffer[4]);

                uint32_t result = (lo >> bitOffset) | (hi << (32 - bitOffset));

                uint32_t mask = (count == 32) ? ~0u : ((1u << count) - 1);
                result &= mask;

                bitPos += count;
                return result;
            }
            int8_t ReadInt8() {
                return (int8_t)ReadUInt8();
            }
            /// <summary>
            /// Reads an unsigned 8-bit integer from the bitstream without checking for buffer limits/overflows.
            /// </summary>
            /// <returns></returns>
            uint8_t ReadUInt8Unchecked() {
                const size_t byteIndex = bitPos >> 3;
                const size_t bitOffset = bitPos & 7;

                const uint16_t word = (uint16_t(data[byteIndex + 1]) << 8) | data[byteIndex];
                const uint8_t ret = static_cast<uint8_t>(word >> bitOffset);

                bitPos += 8;
                return ret;
            }
            /// <summary>
            /// Reads an unsigned 8-bit integer from the bitstream.
            /// </summary>
            /// <returns></returns>
            uint8_t ReadUInt8() {
                const size_t maxBits = dataSizeInBytes * 8;
                if (bitPos + 8 > maxBits) [[unlikely]] {
                    throw std::out_of_range("BitstreamReader [ReadUInt8]: reading past buffer");
                }

                const size_t byteIndex = bitPos >> 3;
                const size_t bitOffset = bitPos & 7;

                uint8_t ret;
                if (bitOffset == 0) {
                    ret = data[byteIndex];
                }else{
                    // since we are not aligned we need to read the extra byte that contains the needed data that may not exist
                    if (byteIndex + 1 >= dataSizeInBytes) [[unlikely]] {
                        throw std::out_of_range("BitstreamReader [ReadUInt8]: unaligned read over buffer edge");
                    }
                    const uint16_t word = (uint16_t(data[byteIndex + 1]) << 8) | data[byteIndex];
                    ret = static_cast<uint8_t>(word >> bitOffset);
                }
                bitPos += 8;
                return ret;
            }
            int16_t ReadInt16() {
                return (int16_t)ReadUInt16();
            }
            uint16_t ReadUInt16() {
                const size_t maxBits = dataSizeInBytes * 8;
                if (bitPos + 16 > maxBits) {
                    throw std::out_of_range("BitstreamReader [ReadUInt16]: reading past buffer");
                }

                size_t byteIndex = bitPos >> 3;
                size_t bitOffset = bitPos & 7;
                uint16_t ret = 0;

                    if (bitOffset == 0) {
                        // Aligned little-endian
                        ret = uint16_t(data[byteIndex]);
                        ret |= uint16_t(data[byteIndex + 1]) << 8;
                    }
                    else {
                        // Unaligned LSB0: 3-byte pack, then shift right
                        uint32_t tmp = 0;
                        tmp |= uint32_t(data[byteIndex]);
                        tmp |= uint32_t(data[byteIndex + 1]) << 8;
                        tmp |= uint32_t(data[byteIndex + 2]) << 16;
                        ret = uint16_t(tmp >> bitOffset);
                    }

                bitPos += 16;
                return ret;
            }
            int32_t ReadInt32(){
                return (int32_t)ReadUInt32();
            }
            uint32_t ReadUInt32() {
                const size_t maxBits = dataSizeInBytes * 8;
                if (bitPos + 32 > maxBits) {
                    throw std::out_of_range("BitstreamReader [ReadUInt32]: reading past buffer");
                }

                size_t byteIndex = bitPos >> 3;
                size_t bitOffset = bitPos & 7;
                uint32_t ret = 0;

                    if (bitOffset == 0) {
                        // Aligned little-endian
                        ret = uint32_t(data[byteIndex]);
                        ret |= uint32_t(data[byteIndex + 1]) << 8;
                        ret |= uint32_t(data[byteIndex + 2]) << 16;
                        ret |= uint32_t(data[byteIndex + 3]) << 24;
                    }else{
                        // Unaligned LSB0: 5-byte pack, then shift right
                        uint64_t tmp = 0;
                        tmp |= uint64_t(data[byteIndex]);
                        tmp |= uint64_t(data[byteIndex + 1]) << 8;
                        tmp |= uint64_t(data[byteIndex + 2]) << 16;
                        tmp |= uint64_t(data[byteIndex + 3]) << 24;
                        tmp |= uint64_t(data[byteIndex + 4]) << 32;
                        ret = uint32_t(tmp >> bitOffset);
                    }

                bitPos += 32;
                return ret;
            }
            std::vector<uint8_t> ReadBytesAligned(int count){
                if (!IsByteAligned()){throw std::out_of_range("BitstreamReader [ReadBytesAligned]: bytes not aligned");}
                std::vector<uint8_t> output;
                output.reserve(count);
                for (int i = 0; i < count; ++i) {
                    output.push_back(ReadBits32(8));
                }
                return output;
            }
            /// <summary>
            /// 
            /// </summary>
            /// <returns></returns>
            bool ReadBool() {
                // Faster than division
                const size_t byteIndex = bitPos >> 3;         
                // Faster than modulo
                const size_t bitIndex = bitPos & 7;          
                // Increment early to enable tail-call optimization
                bitPos++;                                     
                // Bounds check — keep it but consider moving it out if you batch reads
                if (byteIndex >= dataSizeInBytes) {
                    throw std::out_of_range("BitstreamReader [ReadBool]: reading past buffer");
                }
                return (data[byteIndex] >> bitIndex) & 1;
            }
            /// <summary>
            /// 
            /// </summary>
            /// <returns></returns>
            bool ReadBoolUnchecked() {
                const size_t byteIndex = bitPos >> 3;
                const size_t bitIndex = bitPos++ & 7;
                return (data[byteIndex] >> bitIndex) & 1;
            }
            /// <summary>
            /// Reads a null-terminated string from the bitstream, up to a maximum of 'max_len' bytes.
            /// Stops reading when a null character is encountered or the limit is reached.
            /// </summary>
            /// <param name="str"></param>
            /// <param name="max_len"></param>
            void ReadStringNullTerminated(std::string& str,size_t max_len){
                AlignToByte();
                str.clear();
                while (str.length() < max_len && !IsEOF()) {
                    char c = (char)ReadBits32(8);
                    if (c == '\0') {
                        break;
                    }
                    str += c;
                }
            }
            /// <summary>
            /// Reads a string from the bitstream, the string is prefixed by an 32-bit length field.
            /// Limits the output to a maximum of 'max_len' bytes.
            /// </summary>
            /// <param name="str"></param>
            /// <param name="max_len"></param>
            void ReadStringLengthPrefixed32(std::string& str, size_t max_len) {
                AlignToByte();
                size_t length = ReadUInt32();
                if (length > max_len) {
                    throw std::runtime_error("BitstreamReader [ReadLengthPrefixedString]: String length exceeds max_len.");
                }
                if (RemainingBits() < length * 8) {
                    throw std::out_of_range("BitstreamReader [ReadLengthPrefixedString]: Not enough data in buffer to read string.");
                }
                str.clear();
                str.reserve(length);
                for (uint32_t i = 0; i < length; ++i) {
                    str += (char)ReadBits32(8);
                }
            }
            /// <summary>
            /// Reads a string from the bitstream, the string is prefixed by an 8-bit length field.
            /// Limits the output to a maximum of 'max_len' bytes.
            /// </summary>
            /// <param name="str"></param>
            /// <param name="max_len"></param>
            void ReadStringLengthPrefixed8(std::string& str, size_t max_len) {
                AlignToByte();
                size_t length = ReadBits32(8);
                if (length > max_len) {
                    throw std::runtime_error("BitstreamReader [ReadLengthPrefixedString]: String length exceeds max_len.");
                }
                if (RemainingBits() < length * 8) {
                    throw std::out_of_range("BitstreamReader [ReadLengthPrefixedString]: Not enough data in buffer to read string.");
                }
                str.clear();
                str.reserve(length);
                for (uint32_t i = 0; i < length; ++i) {
                    str += (char)ReadBits32(8);
                }
            }
            /// <summary>
            /// Reads the specified number of bits from the stream without changing the position.
            /// </summary>
            /// <param name="count"></param>
            /// <returns></returns>
            uint32_t PeekBits(int count) {
                if (count <= 0 || count > 32)
                    throw std::invalid_argument("BitstreamReader [PeekBits]: count 1–32");

                size_t origPos = bitPos;
                uint32_t v = ReadBits32(count);
                bitPos = origPos;
                return v;
            }
            /// <summary>
            /// Checks if there is enough data to read the specified number of bits.
            /// </summary>
            /// <param name="count"></param>
            /// <returns></returns>
            bool EnsureBits(int count) const{
                return ((bitPos + count) <= (dataSizeInBytes << 3));
            }
            /// <summary>
            /// Checks if there is enough data to read the specified number of bytes.
            /// </summary>
            /// <param name="count">Number of bytes to check for</param>
            /// <returns>True if enough data is available, false otherwise</returns>
            bool EnsureBytes(int count) const {
                return bitPos + (static_cast<size_t>(count) << 3) <= (dataSizeInBytes << 3);
            }
            /// <summary>
            /// Advances the bit position by the specified number of bits.
            /// </summary>
            /// <param name="count"></param>
            void SkipBits(int count) {
                if (count < 0 || (bitPos + count) >(dataSizeInBytes * 8)) {
                    throw std::out_of_range("BitstreamReader [SkipBits] beyond EOF");
                }
                SkipBitsUnchecked(count);
            }
            /// <summary>
            /// Advances the bit position by the specified number of bits without checking if it goes beyond the end of the data.
            /// </summary>
            /// <param name="count"></param>
            void SkipBitsUnchecked(int count) {
                bitPos += count;
            }
            /// <summary>
            /// Advances the bit position by the specified number of bytes.
            /// </summary>
            /// <param name="count"></param>
            void SkipBytes(int count) {
                if (count < 0) {
                    throw std::invalid_argument("BitstreamReader [SkipBytes]: count must be non-negative");
                }
                size_t bitsToSkip = static_cast<size_t>(count) << 3;
                if (bitPos + bitsToSkip > dataSizeInBytes * 8) {
                    throw std::out_of_range("BitstreamReader [SkipBytes]: beyond EOF");
                }
                bitPos += bitsToSkip;
            }
            void SkipBytesUnchecked(int count) {
                bitPos += static_cast<size_t>(count) << 3;
            }
            inline bool IsByteAligned() const {
                return (bitPos & 7) == 0;
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
            /// <summary>
            /// Get remaining bits left.
            /// </summary>
            /// <returns></returns>
            size_t RemainingBits() const {
                // Convert byte size to bit size using a left shift (x8)
                size_t bitLimit = dataSizeInBytes << 3;
                // Compute how many bits remain, assuming bitPos is within bounds
                size_t delta = bitLimit - bitPos;
                // Create a mask: ~0 (all bits set) if bitPos <= bitLimit, 0 otherwise
                // This avoids branching by converting the boolean result into a bitmask
                size_t mask = -(bitPos <= bitLimit);
                // Apply the mask: returns delta if valid, 0 if overflow
                // This is branchless because it uses bitwise AND instead of an if-statement
                return delta & mask;
            }
            /// <summary>
            /// Get remaining bytes left.
            /// </summary>
            /// <returns></returns>
            size_t RemainingBytes() const {
                return RemainingBits() >> 3;
            }
            /// <summary>
            /// Are we at the end of the data?
            /// </summary>
            /// <returns></returns>
            bool IsEOF() const {
                return bitPos >= dataSizeInBytes * 8;
            }

        };
    }
}
#endif