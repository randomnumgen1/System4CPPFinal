#include <cstdint>
#include <vector>
#include <stdexcept>
namespace System {
namespace IO {
namespace Compression {
	//| MagicNb	| F. Descriptor	| Data Block	| (...)	| EndMark	| C. Checksum |
	//| 4 bytes	| 3-15 bytes	|               |       |4 bytes	|  0-4 bytes  |
	class LZ4{
		private:
			struct LZ4Frame {
				uint32_t magic;
				uint8_t FLG;
				uint8_t BD;

			};
			// Determine maximum block size from the BD (Block Descriptor) byte.
            size_t getMaxBlockSize(uint8_t BD);
            // A helper that decompresses one block in LZ4 format.
// 'src' points to the beginning of the block data,
// 'srcSize' is the length of the compressed block,
// 'maxBlockSize' is used to reserve our output buffer.
            std::vector<uint8_t> decompressBlock(const uint8_t* src, size_t srcSize, size_t maxBlockSize) {
                std::vector<uint8_t> dst;
                dst.reserve(maxBlockSize);
                size_t pos = 0;
                const size_t MIN_MATCH = 4;

                while (pos < srcSize) {
                    // Read the token.
                    uint8_t token = src[pos++];
                    // High nibble: literal length.
                    size_t literalLength = token >> 4;
                    if (literalLength == 15) {
                        uint8_t len = 0;
                        while (pos < srcSize && (len = src[pos++]) == 255) {
                            literalLength += 255;
                        }
                        // If the loop exits before reading an extra byte <255, add it.
                        if (pos <= srcSize)
                            literalLength += (src[pos - 1]);
                    }
                    // Copy literal bytes.
                    if (pos + literalLength > srcSize)
                        throw std::runtime_error("Not enough data for literals in compressed block.");
                    dst.insert(dst.end(), src + pos, src + pos + literalLength);
                    pos += literalLength;

                    // If we’ve reached the end of the block, break.
                    if (pos >= srcSize)
                        break;

                    // Read offset (2 bytes, little-endian).
                    if (pos + 2 > srcSize)
                        throw std::runtime_error("Not enough data for offset in compressed block.");
                    uint16_t offset = src[pos] | (src[pos + 1] << 8);
                    pos += 2;
                    if (offset == 0)
                        throw std::runtime_error("0 offset encountered in compressed block.");

                    // Low nibble: match length.
                    size_t matchLength = token & 0x0F;
                    if (matchLength == 15) {
                        uint8_t len = 0;
                        while (pos < srcSize && (len = src[pos++]) == 255) {
                            matchLength += 255;
                        }
                        if (pos <= srcSize)
                            matchLength += src[pos - 1];
                    }
                    matchLength += MIN_MATCH;

                    size_t dstSize = dst.size();
                    if (offset > dstSize)
                        throw std::runtime_error("Offset beyond decompressed data in block.");
                    // Copy the match bytes. (This loop automatically handles overlapping regions.)
                    for (size_t i = 0; i < matchLength; i++) {
                        dst.push_back(dst[dstSize - offset + i]);
                    }
                }
                return dst;
            }
		public:
            std::vector<uint8_t> Decompress(const std::vector<uint8_t>& compressedData);

		
		
	};
}
}
}