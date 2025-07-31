#include <System/Image.hpp>
#include <cstring>

namespace System {
	Image::Image(int w, int h) {
		Width = w;
		Height = h;
		m_pixels.assign(Width * Height * 4, 255);
	}
 

#if defined(SYSTEM_EXPERIMENTAL_ENABLED)
	void Image::LoadFromPNG(const std::string& filename) {
		std::ifstream file(filename, std::ios::binary);
		if (!file) {
			throw std::invalid_argument("couldnt find file");
		}

		char magic[8];
		file.read(magic, 8);
		if (magic[0] != (char)0x89 || magic[1] != 'P' || magic[2] != 'N' || magic[3] != 'G' || magic[4] != (char)0x0D || magic[5] != (char)0x0A || magic[6] != (char)0x1A || magic[7] != (char)0x0A) {
			throw std::invalid_argument("invalid png magic");
		}
		uint32_t calculatedCrc = {};
		System::Security::Cryptography::HashAlgorithm::CRC32 crc32(System::Security::Cryptography::HashAlgorithm::CRC32POLYNOMIAL::ISO_HDLC);
		//load chunk into chunks and verify crc
		std::vector<PNG_CHUNK> chunks;
		while (!file.eof()) {
			PNG_CHUNK chunk;
			chunk.Length = ReadUInt32BigEndian(file);
			file.read(reinterpret_cast<char*>(&chunk.Type), 4);
			//chunk.Type = ReadUInt32BigEndian(file);
			if (chunk.Length > 0) {
				chunk.Data.resize(chunk.Length);
				file.read(reinterpret_cast<char*>(chunk.Data.data()), chunk.Length);
			}
			file.read(reinterpret_cast<char*>(&chunk.CRC), 4);
			//crc is the chunk type + data
			crc32.update(reinterpret_cast<const uint8_t*>(&chunk.Type), 4);
			if (chunk.Data.size() > 0) {
				crc32.update(chunk.Data.data(), chunk.Data.size());
			}
			crc32.finalize(reinterpret_cast<uint8_t*>(&calculatedCrc));
			if (calculatedCrc != chunk.CRC) {
				throw std::invalid_argument("crc32 mismatch.");
			}
			chunks.push_back(chunk);
			if (chunk.GetType() == 0x49454E44) {
				break;
			}
		}
		//IHDR is the first chunk in chunks
		if (chunks[0].GetType() != 0x49484452) {
			throw std::invalid_argument("first chunk not ihdr.");
		}
		PNG_IHDR* ihdr = reinterpret_cast<PNG_IHDR*>(chunks[0].Data.data());
		ihdr->fixendian();
		Width = ihdr->width;
		Height = ihdr->height;
		if (ihdr->bitDepth != 8 || ihdr->colorType != 6) {
			throw std::invalid_argument("Only 8 bpp RGBA (colorType 6) supported.");
		}
		if (ihdr->compression != 0) {
			throw std::invalid_argument("only support deflate compression");
		}
		if (ihdr->filter != 0) {
			throw std::invalid_argument("only support adaptive filtering");
		}
		if (ihdr->interlace != 0) {
			throw std::invalid_argument("we dont support interlace");
		}
		//return early as we don't support zlib compression yet, remember deflate is a compression algorithm, zlib is a file format that uses deflate.
		return;
		std::vector<uint8_t> idat_data;
		for (auto chunk : chunks) {
			if (chunk.GetType() == 0x49444154) { // IDAT
				idat_data.insert(idat_data.end(), chunk.Data.begin(), chunk.Data.end());
			}
			else if (chunk.GetType() == 0x49454E44) { // IEND
				std::cout << "attempting decompress";
				std::vector<uint8_t> decompressed_data = System::IO::Compression::Deflate::Decompress(idat_data);
				Unfilter(decompressed_data, *ihdr);
				break;
			}
		}



		throw std::invalid_argument("");

	}
	void Image::SaveAsPNG(const std::string& filename) {
		std::ofstream file(filename, std::ios::binary);
		if (!file) {
			return;
		}
		//file.write(reinterpret_cast<const char*>(&PNG_MAGIC), sizeof(PNG_MAGIC));

		PNG_CHUNK chunk;
		chunk.Type = 6;
		chunk.Data = { 'H', 'e', 'l', 'l', 'o', '!', '!', '!' };
		std::vector<PNG_CHUNK> Chunks;
		Chunks.push_back(chunk);


		for (int i = 0; i < Chunks.size(); ++i) {
			System::Security::Cryptography::HashAlgorithm::CRC32 crc32;
			crc32.update(reinterpret_cast<const uint8_t*>(&Chunks[i].Type), 4);
			crc32.update(reinterpret_cast<const uint8_t*>(Chunks[i].Data.data()), Chunks[i].Data.size());
			crc32.finalize(reinterpret_cast<uint8_t*>(&Chunks[i].CRC));






			file.write(reinterpret_cast<const char*>(&Chunks[i].Type), 4);
			file.write(reinterpret_cast<const char*>(Chunks[i].Data.data()), Chunks[i].Data.size());
			file.write(reinterpret_cast<const char*>(&Chunks[i].CRC), 4);


		}

	}
#endif


	void Image::SaveAsBitmap(const std::string& filename) {
		std::ofstream file(filename, std::ios::binary);
		if (!file) {
			return;
		}
		BITMAPFILEHEADER bitmapfileheader = {};
		bitmapfileheader.Magic = BITMAP_MAGIC;
		bitmapfileheader.FileSize = (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)) + Width * Height * 4;
		bitmapfileheader.Reserved1 = 0;
		bitmapfileheader.Reserved2 = 0;
		bitmapfileheader.pixeloffset = 54;
		file.write(reinterpret_cast<const char*>(&bitmapfileheader), sizeof(BITMAPFILEHEADER));

		BITMAPINFOHEADER bitmapinfoheader = {};
		bitmapinfoheader.ThisHeaderSize = sizeof(BITMAPINFOHEADER);
		bitmapinfoheader.WidthInPixels = Width;
		bitmapinfoheader.HeightInPixels = Height;
		bitmapinfoheader.Planes = 1;
		bitmapinfoheader.bpp = 32;
		bitmapinfoheader.Compression = 0;
		bitmapinfoheader.ImageSize = Width * Height * 4;
		bitmapinfoheader.XPelsPerMeter = 2835;
		bitmapinfoheader.YPelsPerMeter = 2835;
		bitmapinfoheader.biClrUsed = 0;
		bitmapinfoheader.biClrImportant = 0;
		file.write(reinterpret_cast<const char*>(&bitmapinfoheader), sizeof(BITMAPINFOHEADER));
		// Pixel Data
		uint32_t* pixel_data_rgba = reinterpret_cast<uint32_t*>(m_pixels.data());
		for (int y = Height - 1; y >= 0; --y) {
			for (int x = 0; x < Width; ++x) {
				int idx = (y * Width + x);
				uint32_t pixel = pixel_data_rgba[idx];
				uint32_t bgra = (pixel & 0xFF00FF00) | ((pixel & 0x00FF0000) >> 16) | ((pixel & 0x000000FF) << 16);
				file.write(reinterpret_cast<const char*>(&bgra), sizeof(uint32_t));
			}
		}
		file.close();
	}
	void Image::SaveAsTGA(const std::string& filename) {
		std::ofstream file(filename, std::ios::binary);
		if (!file) {
			return;
		}
		TGAFILEHEADER tgafileheader = {};
		tgafileheader.imageType = 2;//Uncompressed true-color
		tgafileheader.width = Width;
		tgafileheader.height = Height;
		tgafileheader.pixelDepth = 32;// Bits per pixel
		file.write(reinterpret_cast<const char*>(&tgafileheader), sizeof(TGAFILEHEADER));
		// Pixel Data
		uint32_t* pixel_data_rgba = reinterpret_cast<uint32_t*>(m_pixels.data());
		for (int y = Height - 1; y >= 0; --y) {
			for (int x = 0; x < Width; ++x) {
				int idx = (y * Width + x);
				uint32_t pixel = pixel_data_rgba[idx];
				uint32_t bgra = (pixel & 0xFF00FF00) | ((pixel & 0x00FF0000) >> 16) | ((pixel & 0x000000FF) << 16);
				file.write(reinterpret_cast<const char*>(&bgra), sizeof(uint32_t));
			}
		}
		file.close();
	}
	void Image::LoadFromBitmap(const std::string& filename) {
		std::ifstream file(filename, std::ios::binary);
		if (!file) {
			throw std::invalid_argument("couldnt find file");
		}
		BITMAPFILEHEADER bitmapfileheader = {};
		file.read(reinterpret_cast<char*>(&bitmapfileheader), sizeof(BITMAPFILEHEADER));
		if (bitmapfileheader.Magic != BITMAP_MAGIC) {
			throw std::runtime_error("BITMAP_MAGIC");
		}
		BITMAPINFOHEADER bitmapinfoheader = {};
		file.read(reinterpret_cast<char*>(&bitmapinfoheader), sizeof(BITMAPINFOHEADER));

		Width = bitmapinfoheader.WidthInPixels;
		Height = bitmapinfoheader.HeightInPixels;

		if (bitmapinfoheader.bpp != 32) {
			throw std::runtime_error("BITMAP NOT 32 BPP");
		}

		m_pixels.assign(Width * Height * 4, 255);

		// Pixel Data
		file.seekg(bitmapfileheader.pixeloffset, std::ios::beg);
		for (int y = Height - 1; y >= 0; --y) {
			for (int x = 0; x < Width; ++x) {
				int idx = (y * Width + x) * 4;
				// BMP stores pixels in BGRA order
				file.read(reinterpret_cast<char*>(&m_pixels[idx + 2]), 1);
				file.read(reinterpret_cast<char*>(&m_pixels[idx + 1]), 1);
				file.read(reinterpret_cast<char*>(&m_pixels[idx + 0]), 1);
				file.read(reinterpret_cast<char*>(&m_pixels[idx + 3]), 1);

			}
		}
	}
	void Image::LoadFromTGA(const std::string& filename) {
		std::ifstream file(filename, std::ios::binary);
		if (!file) throw std::invalid_argument("couldnt find file");
		TGAFILEHEADER tgafileheader = {};
		file.read(reinterpret_cast<char*>(&tgafileheader), sizeof(TGAFILEHEADER));

		if (tgafileheader.imageType == 2) { // Uncompressed true-color
			Width = tgafileheader.width;
			Height = tgafileheader.height;
			int bpp = tgafileheader.pixelDepth;

			if (bpp == 32) {
				m_pixels.assign(Width * Height * 4, 255);
				for (int y = Height - 1; y >= 0; --y) {
					for (int x = 0; x < Width; ++x) {
						int idx = (y * Width + x) * 4;
						file.read(reinterpret_cast<char*>(&m_pixels[idx + 2]), 1); // B
						file.read(reinterpret_cast<char*>(&m_pixels[idx + 1]), 1); // G
						file.read(reinterpret_cast<char*>(&m_pixels[idx + 0]), 1); // R
						file.read(reinterpret_cast<char*>(&m_pixels[idx + 3]), 1); // A
					}
				}
			}
			else {
				throw std::runtime_error("unsupported tga image bpp");
			}
		}
		else {
			throw std::runtime_error("unsupported tga image type");
		}
	}

	void Image::Flip() {
		const int row_bytes = Width * 4;
		std::vector<uint8_t> temp_row(row_bytes);

		for (int y = 0; y < Height / 2; ++y) {
			uint8_t* top = &m_pixels[y * row_bytes];
			uint8_t* bottom = &m_pixels[(Height - 1 - y) * row_bytes];
			// Copy top row to temp
			std::memcpy(temp_row.data(), top, row_bytes);
			// Copy bottom row to top
			std::memcpy(top, bottom, row_bytes);
			// Copy temp to bottom
			std::memcpy(bottom, temp_row.data(), row_bytes);
		}
	}
	Image::~Image() {
		// Clear vector and ensure raw memory is managed correctly
		m_pixels.clear();
	}

}