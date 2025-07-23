#ifndef _SYSTEM_IMAGE_H
#define _SYSTEM_IMAGE_H


#include "System/Drawing/Size.h"
#include <vector>
#include <iostream>
#include <fstream>

namespace System {

        class Image {
		private:
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
			struct TGAFILEHEADER {
				uint8_t  idLength;         // Length of image ID field
				uint8_t  colorMapType;     // 0 = no color map
				uint8_t  imageType;        // e.g., 2 = uncompressed true-color, 10 = RLE compressed
				uint16_t colorMapStart;    // Index of first color map entry
				uint16_t colorMapLength;   // Total number of color map entries
				uint8_t  colorMapDepth;    // Number of bits per palette entry
				uint16_t xOrigin;          // X origin of image
				uint16_t yOrigin;          // Y origin of image
				uint16_t width;            // Image width in pixels
				uint16_t height;           // Image height in pixels
				uint8_t  pixelDepth;       // Bits per pixel (e.g., 24 or 32)
				uint8_t  imageDescriptor;  // Bit 4-5: origin, Bit 0-3: alpha channel depth
			};
#pragma pack(pop)

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
			static constexpr uint16_t BITMAP_MAGIC = 0x4D42;


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
		 


			void SaveAsTGA(const std::string& filename){
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