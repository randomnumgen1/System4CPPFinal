#ifndef SYSTEM_TOOLS_QRCODE_H
#define SYSTEM_TOOLS_QRCODE_H

#include <System/Image.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <cmath>

namespace System::Images {

    enum class QRECCLevel {
        Low = 0,      // 7%
        Medium = 1,   // 15%
        Quartile = 2, // 25%
        High = 3      // 30%
    };

    class QRCODE {
    public:
        void setECClevel(QRECCLevel level) {
            m_eccLevel = level;
        }

        System::Image Generate(const std::string& str) {
            int version = findBestVersion(str.length(), m_eccLevel);
            if (version > 10) throw std::runtime_error("Data too large for current QR implementation (max version 10)");

            std::vector<uint8_t> dataBytes(str.begin(), str.end());
            std::vector<uint8_t> encodedData = encodeData(dataBytes, version, m_eccLevel);
            std::vector<uint8_t> allCodewords = addErrorCorrection(encodedData, version, m_eccLevel);

            int size = 21 + (version - 1) * 4;
            std::vector<int8_t> matrix(size * size, -1); // -1: empty, 0: white, 1: black
            std::vector<bool> isFunction(size * size, false);

            addFunctionalPatterns(matrix, isFunction, version);
            addDataAndECC(matrix, isFunction, allCodewords, version);

            // We use Mask 0 for simplicity. A full implementation would test all 8 masks.
            int bestMask = 0; 
            applyMask(matrix, isFunction, bestMask);
            
            finalizeMatrix(matrix, version, m_eccLevel, bestMask);

            // Convert to System::Image
            int quietZone = 4;
            int scale = 8; 
            int modulesWithQuiet = size + quietZone * 2;
            int imgSize = modulesWithQuiet * scale;
            System::Image img(imgSize, imgSize);

            // Fill with white (RGBA)
            for (int i = 0; i < (int)img.m_pixels.size(); i += 4) {
                img.m_pixels[i] = 255;
                img.m_pixels[i + 1] = 255;
                img.m_pixels[i + 2] = 255;
                img.m_pixels[i + 3] = 255;
            }

            for (int y = 0; y < size; ++y) {
                for (int x = 0; x < size; ++x) {
                    if (matrix[y * size + x] == 1) {
                        for (int sy = 0; sy < scale; ++sy) {
                            for (int sx = 0; sx < scale; ++sx) {
                                int py = (y + quietZone) * scale + sy;
                                int px = (x + quietZone) * scale + sx;
                                if (py < 0 || py >= imgSize || px < 0 || px >= imgSize) continue;
                                int idx = (py * imgSize + px) * 4;
                                img.m_pixels[idx] = 0;     // R
                                img.m_pixels[idx + 1] = 0; // G
                                img.m_pixels[idx + 2] = 0; // B
                                img.m_pixels[idx + 3] = 255; // A
                            }
                        }
                    }
                }
            }

            return img;
        }

    private:
        QRECCLevel m_eccLevel = QRECCLevel::Low;

        struct VersionInfo {
            int totalCodewords;
            int eccPerBlock[4];
            int numBlocks1[4];
            int dataCodewords1[4];
            int numBlocks2[4];
            int dataCodewords2[4];
        };

        static VersionInfo getVersionInfo(int version) {
            static const VersionInfo table[] = {
                { 0, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} },
                { 26, {7,10,13,17}, {1,1,1,1}, {19,16,13,9}, {0,0,0,0}, {0,0,0,0} }, // V1
                { 44, {10,16,22,28}, {1,1,1,1}, {34,28,22,16}, {0,0,0,0}, {0,0,0,0} }, // V2
                { 70, {15,26,18,22}, {1,1,2,2}, {55,44,17,13}, {0,0,0,0}, {0,0,0,0} }, // V3
                { 100, {20,18,26,16}, {1,2,2,4}, {80,32,24,9}, {0,0,0,0}, {0,0,0,0} }, // V4
                { 134, {26,24,18,22}, {1,2,4,4}, {108,43,15,11}, {0,0,0,0}, {0,0,0,0} }, // V5
                { 172, {18,16,24,28}, {2,4,4,4}, {68,27,19,15}, {0,0,0,0}, {0,0,0,0} }, // V6
                { 196, {20,18,18,26}, {2,4,6,5}, {78,31,14,13}, {0,0,0,0}, {0,0,0,0} }, // V7
                { 242, {24,22,22,26}, {2,4,6,6}, {97,38,18,14}, {0,0,0,0}, {0,0,0,0} }, // V8
                { 292, {30,22,20,24}, {2,5,8,8}, {116,36,14,12}, {0,0,0,0}, {0,0,0,0} }, // V9
                { 346, {18,26,24,28}, {4,5,8,8}, {68,43,22,15}, {0,0,0,0}, {0,0,0,0} }  // V10
            };
            if (version < 1) return table[1];
            if (version > 10) return table[10]; 
            return table[version];
        }

        int findBestVersion(size_t dataSize, QRECCLevel ecc) {
            int eccIdx = static_cast<int>(ecc);
            for (int v = 1; v <= 10; ++v) {
                VersionInfo vi = getVersionInfo(v);
                int capacity = vi.numBlocks1[eccIdx] * vi.dataCodewords1[eccIdx] + vi.numBlocks2[eccIdx] * vi.dataCodewords2[eccIdx];
                int overhead = (v < 10) ? 2 : 3; 
                if (dataSize + overhead <= (size_t)capacity) return v;
            }
            return 11;
        }

        std::vector<uint8_t> encodeData(const std::vector<uint8_t>& data, int version, QRECCLevel ecc) {
            VersionInfo vi = getVersionInfo(version);
            int eccIdx = static_cast<int>(ecc);
            int totalDataCodewords = vi.numBlocks1[eccIdx] * vi.dataCodewords1[eccIdx] + vi.numBlocks2[eccIdx] * vi.dataCodewords2[eccIdx];

            std::vector<uint8_t> bits;
            auto addBits = [&](uint32_t val, int count) {
                for (int i = count - 1; i >= 0; --i) {
                    bits.push_back((val >> i) & 1);
                }
            };

            addBits(4, 4); // Byte mode indicator
            addBits(data.size(), (version < 10) ? 8 : 16);
            for (uint8_t b : data) addBits(b, 8);

            // Terminator
            int termLen = std::min(4, (int)(totalDataCodewords * 8 - (int)bits.size()));
            for (int i = 0; i < termLen; ++i) bits.push_back(0);
            
            // Pad to byte boundary
            while (bits.size() % 8 != 0) bits.push_back(0);

            std::vector<uint8_t> bytes;
            for (size_t i = 0; i < bits.size(); i += 8) {
                uint8_t b = 0;
                for (int j = 0; j < 8; ++j) b = (b << 1) | bits[i + j];
                bytes.push_back(b);
            }

            // Padding codewords
            uint8_t pad[] = { 0xEC, 0x11 };
            int p = 0;
            while (bytes.size() < (size_t)totalDataCodewords) {
                bytes.push_back(pad[p]);
                p = 1 - p;
            }
            return bytes;
        }

        static uint8_t gf_mul(uint8_t a, uint8_t b) {
            if (a == 0 || b == 0) return 0;
            static uint8_t log_table[256];
            static uint8_t exp_table[256];
            static bool init = false;
            if (!init) {
                int x = 1;
                for (int i = 0; i < 255; i++) {
                    exp_table[i] = (uint8_t)x;
                    log_table[x] = (uint8_t)i;
                    x <<= 1;
                    if (x & 0x100) x ^= 0x11D;
                }
                exp_table[255] = exp_table[0];
                init = true;
            }
            int res = (int)log_table[a] + (int)log_table[b];
            return exp_table[res % 255];
        }

        std::vector<uint8_t> rs_encode(const std::vector<uint8_t>& data, int eccCount) {
            std::vector<uint8_t> gen(eccCount + 1);
            gen[0] = 1;
            int x = 1;
            for (int i = 0; i < eccCount; i++) {
                for (int j = i; j >= 0; j--) {
                    gen[j + 1] ^= gf_mul(gen[j], (uint8_t)x);
                }
                // Generate next power of 2 in GF
                int nextX = x << 1;
                if (nextX & 0x100) nextX ^= 0x11D;
                x = nextX;
            }

            std::vector<uint8_t> res(eccCount, 0);
            for (uint8_t b : data) {
                uint8_t feedback = b ^ res[0];
                res.erase(res.begin());
                res.push_back(0);
                for (int i = 0; i < eccCount; i++) {
                    res[i] ^= gf_mul(gen[i + 1], feedback);
                }
            }
            return res;
        }

        std::vector<uint8_t> addErrorCorrection(const std::vector<uint8_t>& data, int version, QRECCLevel ecc) {
            VersionInfo vi = getVersionInfo(version);
            int eccIdx = static_cast<int>(ecc);
            int eccPerBlock = vi.eccPerBlock[eccIdx];
            std::vector<std::vector<uint8_t>> dataBlocks, eccBlocks;
            int offset = 0;
            for (int i = 0; i < vi.numBlocks1[eccIdx]; i++) {
                std::vector<uint8_t> block(data.begin() + offset, data.begin() + offset + vi.dataCodewords1[eccIdx]);
                dataBlocks.push_back(block);
                eccBlocks.push_back(rs_encode(block, eccPerBlock));
                offset += vi.dataCodewords1[eccIdx];
            }
            for (int i = 0; i < vi.numBlocks2[eccIdx]; i++) {
                std::vector<uint8_t> block(data.begin() + offset, data.begin() + offset + vi.dataCodewords2[eccIdx]);
                dataBlocks.push_back(block);
                eccBlocks.push_back(rs_encode(block, eccPerBlock));
                offset += vi.dataCodewords2[eccIdx];
            }

            std::vector<uint8_t> result;
            int maxData = 0;
            for (auto& b : dataBlocks) maxData = std::max(maxData, (int)b.size());
            for (int i = 0; i < maxData; i++) {
                for (auto& block : dataBlocks) {
                    if (i < (int)block.size()) result.push_back(block[i]);
                }
            }
            for (int i = 0; i < eccPerBlock; i++) {
                for (auto& block : eccBlocks) {
                    result.push_back(block[i]);
                }
            }
            return result;
        }

        void addFunctionalPatterns(std::vector<int8_t>& matrix, std::vector<bool>& isFunction, int version) {
            int size = 21 + (version - 1) * 4;
            auto setRect = [&](int x, int y, int w, int h, int val, bool func) {
                for (int i = 0; i < h; i++) {
                    for (int j = 0; j < w; j++) {
                        int py = y + i, px = x + j;
                        if (py < 0 || py >= size || px < 0 || px >= size) continue;
                        matrix[py * size + px] = (int8_t)val;
                        isFunction[py * size + px] = func;
                    }
                }
            };

            auto finder = [&](int x, int y) {
                setRect(x - 1, y - 1, 9, 9, 0, true); // Separator
                setRect(x, y, 7, 7, 1, true);
                setRect(x + 1, y + 1, 5, 5, 0, true);
                setRect(x + 2, y + 2, 3, 3, 1, true);
            };
            finder(0, 0);
            finder(size - 7, 0);
            finder(0, size - 7);

            // Timing patterns
            for (int i = 8; i < size - 8; i++) {
                matrix[6 * size + i] = (i % 2 == 0) ? 1 : 0;
                isFunction[6 * size + i] = true;
                matrix[i * size + 6] = (i % 2 == 0) ? 1 : 0;
                isFunction[i * size + 6] = true;
            }

            // Alignment patterns
            if (version > 1) {
                static const int alignPos[][7] = {
                    {0,0,0,0,0,0,0}, // V0
                    {0,0,0,0,0,0,0}, // V1
                    {6,18,0,0,0,0,0}, // V2
                    {6,22,0,0,0,0,0}, // V3
                    {6,26,0,0,0,0,0}, // V4
                    {6,30,0,0,0,0,0}, // V5
                    {6,34,0,0,0,0,0}, // V6
                    {6,22,38,0,0,0,0}, // V7
                    {6,24,42,0,0,0,0}, // V8
                    {6,26,46,0,0,0,0}, // V9
                    {6,28,50,0,0,0,0}  // V10
                };
                for (int i = 0; i < 7 && alignPos[version][i] != 0; i++) {
                    for (int j = 0; j < 7 && alignPos[version][j] != 0; j++) {
                        int r = alignPos[version][i];
                        int c = alignPos[version][j];
                        // Skip if it overlaps with finder patterns
                        if (isFunction[r * size + c]) continue;
                        setRect(c - 2, r - 2, 5, 5, 1, true);
                        setRect(c - 1, r - 1, 3, 3, 0, true);
                        matrix[r * size + c] = 1;
                    }
                }
            }

            // Dark module
            matrix[(4 * version + 9) * size + 8] = 1;
            isFunction[(4 * version + 9) * size + 8] = true;

            // Reserve Format info space
            setRect(0, 8, 9, 1, 0, true);
            setRect(8, 0, 1, 8, 0, true);
            setRect(size - 8, 8, 8, 1, 0, true);
            setRect(8, size - 8, 1, 8, 0, true);
        }

        void addDataAndECC(std::vector<int8_t>& matrix, const std::vector<bool>& isFunction, const std::vector<uint8_t>& data, int version) {
            int size = 21 + (version - 1) * 4;
            int bitIdx = 0;
            int totalBits = (int)data.size() * 8;
            bool upward = true;
            for (int x = size - 1; x > 0; x -= 2) {
                if (x == 6) x--;
                for (int y = 0; y < size; y++) {
                    int ry = upward ? (size - 1 - y) : y;
                    for (int dx = 0; dx < 2; dx++) {
                        int px = x - dx;
                        if (!isFunction[ry * size + px]) {
                            int bit = 0;
                            if (bitIdx < totalBits) {
                                bit = (data[bitIdx / 8] >> (7 - (bitIdx % 8))) & 1;
                                bitIdx++;
                            }
                            matrix[ry * size + px] = (int8_t)bit;
                        }
                    }
                }
                upward = !upward;
            }
        }

        void applyMask(std::vector<int8_t>& matrix, const std::vector<bool>& isFunction, int mask) {
            int size = (int)std::sqrt((double)matrix.size());
            for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                    if (!isFunction[y * size + x]) {
                        bool invert = false;
                        switch (mask) {
                            case 0: invert = (x + y) % 2 == 0; break;
                            case 1: invert = y % 2 == 0; break;
                            case 2: invert = x % 3 == 0; break;
                            case 3: invert = (x + y) % 3 == 0; break;
                            case 4: invert = (y / 2 + x / 3) % 2 == 0; break;
                            case 5: invert = ((x * y) % 2) + ((x * y) % 3) == 0; break;
                            case 6: invert = (((x * y) % 2) + ((x * y) % 3)) % 2 == 0; break;
                            case 7: invert = (((x + y) % 2) + ((x * y) % 3)) % 2 == 0; break;
                        }
                        if (invert) matrix[y * size + x] ^= 1;
                    }
                }
            }
        }

        void finalizeMatrix(std::vector<int8_t>& matrix, int version, QRECCLevel ecc, int mask) {
            int size = 21 + (version - 1) * 4;
            
            // Format info (includes ECC level and mask pattern)
            // ECC levels: L=01, M=00, Q=11, H=10
            int eccBits = 0;
            switch (ecc) {
                case QRECCLevel::Low: eccBits = 1; break;
                case QRECCLevel::Medium: eccBits = 0; break;
                case QRECCLevel::Quartile: eccBits = 3; break;
                case QRECCLevel::High: eccBits = 2; break;
            }
            
            uint32_t data = (eccBits << 3) | (mask & 0x07);
            uint32_t rem = data << 10;
            uint32_t poly = 0x537; // 1010011011
            for (int i = 4; i >= 0; i--) {
                if ((rem >> (10 + i)) & 1) rem ^= (poly << i);
            }
            uint32_t formatInfo = (data << 10) | rem;
            formatInfo ^= 0x5412;

            for (int i = 0; i < 15; i++) {
                int bit = (formatInfo >> i) & 1;
                // Vertical
                if (i < 6) matrix[i * size + 8] = (int8_t)bit;
                else if (i < 8) matrix[(i + 1) * size + 8] = (int8_t)bit;
                else matrix[(size - 15 + i) * size + 8] = (int8_t)bit;

                // Horizontal
                if (i < 8) matrix[8 * size + (size - 1 - i)] = (int8_t)bit;
                else if (i < 9) matrix[8 * size + 15 - i] = (int8_t)bit;
                else matrix[8 * size + (14 - i)] = (int8_t)bit;
            }
            // Always dark module
            matrix[(size - 8) * size + 8] = 1;
        }
    };
}

#endif
