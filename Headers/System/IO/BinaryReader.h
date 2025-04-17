#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>
namespace System {
    namespace IO {
        class BinaryReader {
        private:
            std::istream* mStream = nullptr;
        public:
            BinaryReader(std::string filePath) {
                mStream = new std::ifstream(filePath, std::ios::in | std::ios::binary);
                if (!mStream) {
                    throw std::runtime_error("Could not open file");
                }
            }
            ~BinaryReader() {
                if (mStream) {
                    static_cast<std::ifstream*>(mStream)->close();
                    delete mStream;
                }
            }
            /*Int16 Short*/
            int16_t ReadInt16() {
                int16_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), 2);
                return val;
            }
            int16_t ReadShort() {
                int16_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), 2);
                return val;
            }
            uint16_t ReadUInt16() {
                uint16_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), 2);
                return val;
            }
            uint16_t ReadUShort() {
                uint16_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), 2);
                return val;
            }
            uint8_t ReadByte() {
                uint8_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), 1);
                return val;
            }
            int8_t ReadSByte() {
                int8_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), 1);
                return val;
            }
            int32_t ReadInt32() {
                int32_t value = 0;
                if (!mStream->read(reinterpret_cast<char*>(&value), 4)) {
                    throw std::runtime_error("Error reading int32 from stream");
                }
                return value;
            }
            uint32_t ReadUInt32() {
                uint32_t val = 0;
                if (!mStream->read(reinterpret_cast<char*>(&val), 4)) {
                    throw std::runtime_error("Error reading int32 from stream");
                }
                return val;
            }
            int64_t ReadInt64() {
                int64_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), 8);
                return val;
            }
            uint64_t ReadUInt64() {
                uint64_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), 8);
                return val;
            }
            int64_t ReadLong() {
                int64_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), 8);
                return val;
            }
            uint64_t ReadULong() {
                uint64_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), 8);
                return val;
            }
            bool ReadBoolean() {
                uint8_t val = 0;
                mStream->read(reinterpret_cast<char*>(&val), sizeof(val));
                return val != 0;
            }
            std::vector<char> ReadBytes(int32_t size) {
                std::vector<char> buffer(size);
                mStream->read(buffer.data(), size);
                return buffer;
            }
            uint64_t GetPosition() {
                std::streampos pos = mStream->tellg();
                return static_cast<uint64_t>(pos);
            }
            int32_t Read7BitEncodedInt() {
                int32_t value = 0;
                int32_t shift = 0;
                uint8_t byteRead = 0;
                while (true) {
                    if (!mStream->read(reinterpret_cast<char*>(&byteRead), sizeof(byteRead))) {
                        throw std::runtime_error("Error reading 7-bit encoded int from stream");
                    }
                    value |= (static_cast<int32_t>(byteRead & 0x7F)) << shift;
                    if ((byteRead & 0x80) == 0) {
                        break;
                    }
                    shift += 7;
                    if (shift >= 32) {
                        throw std::overflow_error("7-bit encoded integer is too large");
                    }
                }
                return value;
            }
            int64_t Read7BitEncodedInt64() {
                int64_t value = 0;
                int32_t shift = 0;
                uint8_t byteRead = 0;
                while (true) {
                    if (!mStream->read(reinterpret_cast<char*>(&byteRead), sizeof(byteRead))) {
                        throw std::runtime_error("Error reading 7-bit encoded int64 from stream");
                    }
                    value |= (static_cast<int64_t>(byteRead & 0x7F)) << shift;
                    if ((byteRead & 0x80) == 0) {
                        break;
                    }
                    shift += 7;
                    if (shift >= 64) {
                        throw std::overflow_error("7-bit encoded int64 is too large");
                    }
                }
                return value;
            }
            void Seek(std::streamoff offset, std::ios_base::seekdir direction) {
                mStream->seekg(offset, direction);
                if (mStream->fail()) {
                    throw std::runtime_error("Error seeking in stream");
                }
            }





        };
    }
}