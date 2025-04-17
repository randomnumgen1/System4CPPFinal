#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>
namespace System {
    namespace IO {
        class BinaryWriter {
        private:
            std::ostream* mStream = nullptr;

        public:
            BinaryWriter(std::string filePath) {
                mStream = new std::ofstream(filePath, std::ios::out | std::ios::binary);
                if (!mStream) {
                    throw std::runtime_error("Could not open file");
                }
            }
            ~BinaryWriter() {
                if (mStream) {
                    static_cast<std::ofstream*>(mStream)->close();
                    delete mStream;
                }
            }
            void WriteInt16(int16_t value) {
                mStream->write(reinterpret_cast<const char*>(&value), sizeof(value));
            }
            void WriteShort(int16_t value) {
                WriteInt16(value);
            }

            void WriteUInt16(uint16_t value) {
                mStream->write(reinterpret_cast<const char*>(&value), sizeof(value));
            }

            void WriteUShort(uint16_t value) {
                WriteUInt16(value);
            }

            void WriteByte(uint8_t value) {
                mStream->write(reinterpret_cast<const char*>(&value), sizeof(value));
            }

            void WriteSByte(int8_t value) {
                WriteByte(static_cast<uint8_t>(value));
            }

            void WriteInt32(int32_t value) {
                mStream->write(reinterpret_cast<const char*>(&value), sizeof(value));
            }

            void WriteUInt32(uint32_t value) {
                mStream->write(reinterpret_cast<const char*>(&value), sizeof(value));
            }

            void WriteInt64(int64_t value) {
                mStream->write(reinterpret_cast<const char*>(&value), sizeof(value));
            }

            void WriteUInt64(uint64_t value) {
                mStream->write(reinterpret_cast<const char*>(&value), sizeof(value));
            }

            void WriteLong(int64_t value) {
                WriteInt64(value);
            }

            void WriteULong(uint64_t value) {
                WriteUInt64(value);
            }

            void WriteBoolean(bool value) {
                uint8_t byte = static_cast<uint8_t>(value);
                mStream->write(reinterpret_cast<const char*>(&byte), sizeof(byte));
            }

            void WriteBytes(const std::vector<char>& data) {
                mStream->write(data.data(), data.size());
            }

            void Write7BitEncodedInt(int32_t value) {
                while ((value & ~0x7F) != 0) {
                    WriteByte(static_cast<uint8_t>((value & 0x7F) | 0x80));
                    value >>= 7;
                }
                WriteByte(static_cast<uint8_t>(value));
            }
            void Write7BitEncodedInt64(int64_t value) {
                while ((value & ~0x7F) != 0) {
                    WriteByte(static_cast<uint8_t>((value & 0x7F) | 0x80));
                    value >>= 7;
                }
                WriteByte(static_cast<uint8_t>(value));
            }
            void Seek(std::streamoff offset, std::ios_base::seekdir direction) {
                mStream->seekp(offset, direction);
                if (mStream->fail()) {
                    throw std::runtime_error("Error seeking in stream");
                }
            }
        };
    }
}