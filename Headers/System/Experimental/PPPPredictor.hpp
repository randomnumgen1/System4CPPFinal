#include <cstdint>
#include <vector>
#include <cstring> // for memcpy if needed

class PPPPredictor {
private:
    uint8_t GuessTable[1 << 16];
    uint16_t Hash;


public:
    PPPPredictor() {
        std::fill(GuessTable, GuessTable + (1 << 16), 0);
        Hash = 0;
    }
    std::vector<uint8_t> Compress(const std::vector<uint8_t>& source) {
        std::vector<uint8_t> dest;
        // Overestimate
        int estimatedsize = std::max<int>(source.size() / 8 * 9 + 9,((source.size() + 7) / 8) * 9);
        dest.reserve(estimatedsize);

        const uint8_t* source_ptr = source.data();
        size_t len = source.size();
        size_t src_pos = 0;

        while (src_pos < len) {
            size_t flag_pos = dest.size();
            // Placeholder for flags
            dest.push_back(0); 
            uint8_t flags = 0;

            for (int i = 0, bitmask = 1; i < 8 && src_pos < len; ++i, bitmask <<= 1) {
                uint8_t value = source_ptr[src_pos];

                if (GuessTable[Hash] == value) {
                    // Correct guess
                    flags |= bitmask; 
                }else {
                    GuessTable[Hash] = value;
                    // Store mismatched byte
                    dest.push_back(value); 
                }
                Hash = (Hash << 4) ^ (value);
                ++src_pos;
            }

            dest[flag_pos] = flags; // Insert flags at reserved slot
        }

        return dest;
    }
    std::vector<uint8_t> Decompress(const std::vector<uint8_t>& source) {
        std::vector<uint8_t> dest;
        const uint8_t* source_ptr = source.data();
        size_t len = source.size();
        size_t src_pos = 0;

        while (src_pos < len) {
            uint8_t flags = source_ptr[src_pos++];

            for (int i = 0, bitmask = 1; i < 8 && src_pos < len; ++i, bitmask <<= 1) {
                if ((flags & bitmask)) {
                    uint8_t value = GuessTable[Hash];
                    dest.push_back(value);
                    Hash = (Hash << 4) ^ (value);
                }
                else {
                    uint8_t value = source_ptr[src_pos++];
                    dest.push_back(value);
                    GuessTable[Hash] = value;
                    Hash = (Hash << 4) ^ (value);
                }
            }
        }

        return dest;
    }
};