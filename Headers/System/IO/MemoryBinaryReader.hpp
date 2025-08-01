#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>

#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept>

namespace System {
    namespace IO {
        class MemoryBinaryReader {
        private:
            const uint8_t* m_data;
            size_t m_length;
            size_t m_position;

        public:
            MemoryBinaryReader(const uint8_t* data, size_t length)
                : m_data(data), m_length(length), m_position(0) {}

            uint8_t ReadByte() {
                if (m_position >= m_length)
                    throw std::runtime_error("ReadByte: Out of bounds");
                return m_data[m_position++];
            }

            int8_t ReadSByte() {
                return static_cast<int8_t>(ReadByte());
            }

            uint16_t ReadUInt16() {
                if (m_position + 1 >= m_length)
                    throw std::runtime_error("ReadUInt16: Out of bounds");
                uint16_t val = m_data[m_position] | (m_data[m_position + 1] << 8);
                m_position += 2;
                return val;
            }

            int16_t ReadInt16() {
                return static_cast<int16_t>(ReadUInt16());
            }

            uint32_t ReadUInt32() {
                if (m_position + 3 >= m_length)
                    throw std::runtime_error("ReadUInt32: Out of bounds");
                uint32_t val = 0;
                for (int i = 0; i < 4; ++i) {
                    val |= (static_cast<uint32_t>(m_data[m_position++]) << (8 * i));
                }
                return val;
            }

            int32_t ReadInt32() {
                return static_cast<int32_t>(ReadUInt32());
            }

            bool ReadBoolean() {
                return ReadByte() != 0;
            }

            std::vector<uint8_t> ReadBytes(size_t size) {
                if (m_position + size > m_length)
                    throw std::runtime_error("ReadBytes: Out of bounds");
                std::vector<uint8_t> buffer(m_data + m_position, m_data + m_position + size);
                m_position += size;
                return buffer;
            }

            size_t GetPosition() const {
                return m_position;
            }

            void Seek(size_t offset) {
                if (offset >= m_length)
                    throw std::runtime_error("Seek: Out of bounds");
                m_position = offset;
            }
            int32_t Read7BitEncodedInt(){
            
            }
            int64_t Read7BitEncodedInt64(){
            
            }
        };
    }
}
