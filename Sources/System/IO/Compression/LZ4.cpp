#include <System/IO/Compression/LZ4.hpp>

size_t System::IO::Compression::LZ4::getMaxBlockSize(uint8_t BD){
   uint8_t blockSizeIndicator = BD >> 4;
        size_t maxBlockSize = 0;
        switch (blockSizeIndicator) {
        case 4: maxBlockSize = static_cast<size_t>(64) * 1024; break;
        case 5: maxBlockSize = static_cast<size_t>(256) * 1024; break;
        case 6: maxBlockSize = static_cast<size_t>(1024) * 1024; break;
        case 7: maxBlockSize = static_cast<size_t>(4) * 1024 * 1024; break;
        default:
            throw std::runtime_error("Invalid block size indicator in BD field.");
        }
        return maxBlockSize;
}
std::vector<uint8_t> System::IO::Compression::LZ4::Decompress(const std::vector<uint8_t>& compressedData){
    constexpr uint32_t LZ4_MAGIC = 0x184D2204;
    // Ensure we have at least the fixed header.
    if (compressedData.size() < sizeof(LZ4Frame))
        throw std::runtime_error("Compressed data is too small for LZ4 header.");

    // Interpret the first bytes as our fixed header.
    const LZ4Frame* frame = reinterpret_cast<const LZ4Frame*>(compressedData.data());
    if (frame->magic != LZ4_MAGIC)
        throw std::runtime_error("LZ4 Frame Magic error!");

    // Validate frame version. The lower 2 bits of FLG must equal '01'.
    if ((frame->FLG & 0x03) != 0x01)
        throw std::runtime_error("Unsupported LZ4 frame version.");

    // Extract header flags.
//Frame Descriptor FLG  - Block Independence (bit 2)
    bool blockIndependence = (frame->FLG & 0x04) != 0;
    //Frame Descriptor FLG - Block Checksum (bit 3)
    bool blockChecksum = (frame->FLG & 0x08) != 0;
    //Frame Descriptor FLG - Content Size flag (bit 5)
    bool hasContentSize = (frame->FLG & 0x20) != 0;
    //Frame Descriptor FLG - Content Checksum flag (bit 6)
    bool hasContentChecksum = (frame->FLG & 0x40) != 0;
    //Frame Descriptor FLG - Dictionary ID flag (bit 7)
    bool hasDictID = (frame->FLG & 0x80) != 0;

    size_t pos = sizeof(LZ4Frame);
    // Optional: Read content size (8 bytes).
    uint64_t contentSize = 0;
    if (hasContentSize) {
        if (compressedData.size() < pos + 8)
            throw std::runtime_error("Not enough data for Content Size field.");
        std::memcpy(&contentSize, &compressedData[pos], 8);
        pos += 8;
    }
    // Optional: Read Dictionary ID (4 bytes).
    uint32_t dictID = 0;
    if (hasDictID) {
        if (compressedData.size() < pos + 4)
            throw std::runtime_error("Not enough data for Dictionary ID field.");
        std::memcpy(&dictID, &compressedData[pos], 4);
        pos += 4;
    }
    // Read the Header Checksum (1 byte).
    if (compressedData.size() < pos + 1)
        throw std::runtime_error("Missing header checksum.");
    uint8_t headerChecksum = compressedData[pos++];
    // (A full implementation would compute and verify this checksum)

    // Determine maximum block size from the BD byte.
    size_t maxBlockSize = getMaxBlockSize(frame->BD);

    // For simplicity, we only support block-independent frames.
    if (!blockIndependence)
        throw std::runtime_error("Non block-independent frames are not supported.");

    std::vector<uint8_t> decompressedData;

    // Process each data block until we hit a zero block size (end marker).
    while (pos < compressedData.size()) {
        // Each block starts with a 4-byte size field.
        if (pos + 4 > compressedData.size())
            throw std::runtime_error("Incomplete block size field.");
        uint32_t blockSizeField = compressedData[pos] |
            (compressedData[pos + 1] << 8) |
            (compressedData[pos + 2] << 16) |
            (compressedData[pos + 3] << 24);
        pos += 4;

        // A block size of 0 indicates the end of the block section.
        if (blockSizeField == 0)
            break;

        // The MSB of the block size indicates if the block is stored uncompressed.
        bool uncompressed = (blockSizeField & 0x80000000) != 0;
        uint32_t blockSize = blockSizeField & 0x7FFFFFFF;

        if (blockSize > maxBlockSize)
            throw std::runtime_error("Block size exceeds maximum permitted by BD field.");
        if (pos + blockSize > compressedData.size())
            throw std::runtime_error("Incomplete block data.");

        if (uncompressed) {
            // Uncompressed block: append directly to output.
            decompressedData.insert(decompressedData.end(),
                                    compressedData.begin() + pos,
                                    compressedData.begin() + pos + blockSize);
        }
        else {
            // Compressed block: decompress using our helper.
            std::vector<uint8_t> uncompressedBlock =
                decompressBlock(&compressedData[pos], blockSize, maxBlockSize);
            decompressedData.insert(decompressedData.end(),
                                    uncompressedBlock.begin(),
                                    uncompressedBlock.end());
        }
        pos += blockSize;

        // If the Block Checksum flag is enabled, skip 4 bytes (checksum) for this block.
        if (blockChecksum) {
            if (pos + 4 > compressedData.size())
                throw std::runtime_error("Missing block checksum.");
            pos += 4;
        }
    }

    // Optionally, if the Content Checksum flag is set, skip the final 4-byte checksum.
    if (hasContentChecksum) {
        if (pos + 4 > compressedData.size())
            throw std::runtime_error("Missing content checksum.");
        pos += 4;
    }

    // If the header provided a content size, you might verify it here:
    if (hasContentSize && decompressedData.size() != contentSize) {
        throw std::runtime_error("Decompressed size does not match content size declared in header.");
    }

    return decompressedData;
}
