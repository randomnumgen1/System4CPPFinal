#include <cstdint>
#include <vector>
#include <iostream>
//Predictor-1 logic
class PPPPredictor {
private:
    // Predictor lookup table for byte guesses
    uint8_t guessTable[65536] = { 0 };

public:
    std::vector<uint8_t>  decompress(const std::vector<uint8_t>& source) {
        std::vector<uint8_t> dest;
        size_t len = source.size();      // Total bytes left
        size_t pos = 0;                  // Current source position
        uint16_t hash = 0;              // 16-bit rolling hash

        while (pos < len) {
            uint8_t flags = source[pos++]; // Get flags for the frame

            for (int i = 0, bitmask = 1; i < 8; ++i, bitmask <<= 1) {
                if (pos >= len && !(flags & bitmask)) break; // Stop if no more data to read

                uint8_t current;
                if ((flags & bitmask) != 0) {
                    current = guessTable[hash];    // Prediction was correct
                }
                else {
                    if (pos < len) {
                        current = source[pos++];       // Prediction was wrong, read byte
                        guessTable[hash] = current;    // Update guess table
                    }
                    else {
                        break; // Avoid reading past the end of the source vector
                    }
                }

                dest.push_back(current);
                hash = (hash << 4) ^ current;      // Update hash
            }
        }

        return dest;
    }
    std::vector<uint8_t> compress(const std::vector<uint8_t>& source) {
        std::vector<uint8_t> dest;
        size_t len = source.size();      // Total bytes left
        size_t pos = 0;                  // Current source position
        uint16_t hash = 0;              // 16-bit rolling hash

        // Process in frames of 8 bytes
        while (len > 0) {
            uint8_t flags = 0;                     // Bitfield for guesses
            size_t flagIndex = dest.size();        // Reserve spot for flags
            dest.push_back(0);                     // Placeholder for flags

            int count = std::min<int>(8, static_cast<int>(len)); // Bound frame to remaining bytes

            for (int i = 0, bitmask = 1; i < count; ++i, bitmask <<= 1) {
                uint8_t current = source[pos];     // Current byte to predict

                // Check prediction success
                if (guessTable[hash] == current) {
                    flags |= bitmask;              // Prediction succeeded
                    std::cout << "Guess was right at " << pos << std::endl;
                }
                else {
                    guessTable[hash] = current;    // Update guess table
                    dest.push_back(current);       // Output incorrect prediction
                    std::cout << "Guess was wrong at " << pos << std::endl;
                }

                // Update hash with current byte
                hash = (hash << 4) ^ current;

                ++pos;
                --len;
            }

            dest[flagIndex] = flags;               // Fill in flag byte
        }

        return dest;
    }
};