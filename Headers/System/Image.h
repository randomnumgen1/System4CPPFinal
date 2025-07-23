#ifndef _SYSTEM_IMAGE_H
#define _SYSTEM_IMAGE_H


#include "System/Drawing/Size.h"
#include <vector>
#include <iostream>
#include <fstream>

namespace System {

        class Image {
        public:
            union {
                struct {
                    int32_t Width;
                    int32_t Height;
                };
                System::Drawing::Size Size;
            };
            std::vector<uint8_t> m_pixels;
			Image(int w, int h) {
				Width = w;
				Height = h;
				m_pixels.assign(Width * Height * 4, 255);
			}
			enum ImageFormat {
				BMP,
				TGA
			};
			void Load(const std::string& filename) {
				std::string ext = filename.substr(filename.find_last_of(".") + 1);
				if (ext == "bmp") {
					LoadFromBitmap(filename);
				}
				else if (ext == "tga") {
					LoadFromTGA(filename);
				}
			}

			void Save(const std::string& filename, ImageFormat format) {
				switch (format) {
				case BMP:
					SaveAsBitmap(filename);
					break;
				case TGA:
					SaveAsTGA(filename);
					break;
				}
			}
			const uint16_t BITMAP_MAGIC = 0x4D42;
#pragma pack(push, 1)
			struct BITMAPFILEHEADER {
				uint16_t Magic;      // File type; must be 'BM' (0x4D42)
				uint32_t FileSize;      // Size of the file in bytes
				uint16_t Reserved1; // Reserved; must be 0
				uint16_t Reserved2; // Reserved; must be 0
				uint32_t pixeloffset;   // Offset to pixel data from start of file
			};
			struct BITMAPINFOHEADER {
				uint32_t ThisHeaderSize;          // Size of this header (40 bytes)
				int32_t  WidthInPixels;         // Image width in pixels
				int32_t  HeightInPixels;        // Image height in pixels
				uint16_t Planes;        // Must be 1
				uint16_t bpp;      // Bits per pixel (e.g., 24 for truecolor)
				uint32_t Compression;   // Compression type (usually 0 = BI_RGB)
				uint32_t ImageSize;     // Image size (can be 0 for BI_RGB)
				int32_t  XPelsPerMeter; // Horizontal resolution (pixels/meter)
				int32_t  YPelsPerMeter; // Vertical resolution (pixels/meter)
				uint32_t biClrUsed;       // Number of colors in palette
				uint32_t biClrImportant;  // Important colors
			};
#pragma pack(pop)

			void SaveAsBitmap(const std::string& filename){
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
				for (int y = Height - 1; y >= 0; --y) {
					for (int x = 0; x < Width; ++x) {
						int idx = (y * Width + x) * 4;
						// BMP expects BGR order
					


						uint8_t r = m_pixels[idx + 0];
						uint8_t g = m_pixels[idx + 1];
						uint8_t b = m_pixels[idx + 2];
						uint8_t a = m_pixels[idx + 3];
						uint32_t bgra = (b) | (g << 8) | (r << 16) | (a << 24);
						file.write(reinterpret_cast<const char*>(&bgra), sizeof(uint32_t));

					}
				}
				file.close();
			}
		 
			void SaveAsTGA(const std::string& filename) {
				std::ofstream file(filename, std::ios::binary);
				if (!file) return;

				uint8_t header[18] = {};
				header[2] = 2; // Uncompressed true-color
				header[12] = Width & 0xFF;
				header[13] = (Width >> 8) & 0xFF;
				header[14] = Height & 0xFF;
				header[15] = (Height >> 8) & 0xFF;
				header[16] = 32; // Bits per pixel
				file.write(reinterpret_cast<char*>(header), sizeof(header));

				for (int y = Height - 1; y >= 0; --y) {
					for (int x = 0; x < Width; ++x) {
						int idx = (y * Width + x) * 4;
						file.put(m_pixels[idx + 2]); // B
						file.put(m_pixels[idx + 1]); // G
						file.put(m_pixels[idx + 0]); // R
						file.put(m_pixels[idx + 3]); // A
					}
				}
				file.close();
				return;
			}



			void LoadFromBitmap(const std::string& filename) {
				std::ifstream file(filename, std::ios::binary);
				if (!file) {
					return;
				}

				// BMP Header
				char bmpHeader[14];
				file.read(bmpHeader, 14);

				if (bmpHeader[0] != 'B' || bmpHeader[1] != 'M') {
					return;
				}

				// DIB Header
				char dibHeader[40];
				file.read(dibHeader, 40);

				Width = *reinterpret_cast<int32_t*>(&dibHeader[4]);
				Height = *reinterpret_cast<int32_t*>(&dibHeader[8]);
				int16_t bpp = *reinterpret_cast<int16_t*>(&dibHeader[14]);

				if (bpp != 32) {
					return;
				}

				m_pixels.assign(Width * Height * 4, 255);

				// Pixel Data
				file.seekg(*reinterpret_cast<uint32_t*>(&bmpHeader[10]), std::ios::beg);
				for (int y = Height - 1; y >= 0; --y) {
					for (int x = 0; x < Width; ++x) {
						int idx = (y * Width + x) * 4;
						// BMP stores pixels in BGR order
						file.read(reinterpret_cast<char*>(&m_pixels[idx + 2]), 1);
						file.read(reinterpret_cast<char*>(&m_pixels[idx + 1]), 1);
						file.read(reinterpret_cast<char*>(&m_pixels[idx + 0]), 1);
						file.read(reinterpret_cast<char*>(&m_pixels[idx + 3]), 1);
					}
				}
			}
			void LoadFromTGA(const std::string& filename) {
				std::ifstream file(filename, std::ios::binary);
				if (!file) return;

				uint8_t header[18];
				file.read(reinterpret_cast<char*>(header), sizeof(header));

				if (header[2] == 2) { // Uncompressed true-color
					Width = (header[13] << 8) | header[12];
					Height = (header[15] << 8) | header[14];
					int bpp = header[16];

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
				}
			}







			

            ~Image() {
                // Clear vector and ensure raw memory is managed correctly
                m_pixels.clear();
            }
        };
    }
#endif