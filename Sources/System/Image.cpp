#include <System/Image.hpp>
namespace System {
	Image::Image(int w, int h) {
		Width = w;
		Height = h;
		m_pixels.assign(Width * Height * 4, 255);
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
		for (int y = 0; y < Height / 2; ++y) {
			for (int x = 0; x < Width; ++x) {
				int top_idx = (y * Width + x) * 4;
				int bottom_idx = ((Height - 1 - y) * Width + x) * 4;
				for (int i = 0; i < 4; ++i) {
					std::swap(m_pixels[top_idx + i], m_pixels[bottom_idx + i]);
				}
			}
		}
	}
	Image::~Image() {
		// Clear vector and ensure raw memory is managed correctly
		m_pixels.clear();
	}

}