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




			void SaveAsBitmap(const std::string& filename) {
				std::ofstream file(filename, std::ios::binary);
				if (!file) {
					return;
				}

				// BMP Header
				const int fileSize = 54 + Width * Height * 4;
				const int reserved = 0;
				const int dataOffset = 54;

				file.put('B');
				file.put('M');
				file.write(reinterpret_cast<const char*>(&fileSize), 4);
				file.write(reinterpret_cast<const char*>(&reserved), 4);
				file.write(reinterpret_cast<const char*>(&dataOffset), 4);

				// DIB Header
				const int dibHeaderSize = 40;
				const int planes = 1;
				const int bpp = 32;
				const int compression = 0;
				const int imageSize = Width * Height * 4;
				const int xPelsPerMeter = 2835; // 72 DPI
				const int yPelsPerMeter = 2835; // 72 DPI
				const int clrUsed = 0;
				const int clrImportant = 0;

				file.write(reinterpret_cast<const char*>(&dibHeaderSize), 4);
				file.write(reinterpret_cast<const char*>(&Width), 4);
				file.write(reinterpret_cast<const char*>(&Height), 4);
				file.write(reinterpret_cast<const char*>(&planes), 2);
				file.write(reinterpret_cast<const char*>(&bpp), 2);
				file.write(reinterpret_cast<const char*>(&compression), 4);
				file.write(reinterpret_cast<const char*>(&imageSize), 4);
				file.write(reinterpret_cast<const char*>(&xPelsPerMeter), 4);
				file.write(reinterpret_cast<const char*>(&yPelsPerMeter), 4);
				file.write(reinterpret_cast<const char*>(&clrUsed), 4);
				file.write(reinterpret_cast<const char*>(&clrImportant), 4);

				// Pixel Data
				for (int y = Height - 1; y >= 0; --y) {
					for (int x = 0; x < Width; ++x) {
						int idx = (y * Width + x) * 4;
						// BMP expects BGR order
						file.put(m_pixels[idx + 2]);
						file.put(m_pixels[idx + 1]);
						file.put(m_pixels[idx + 0]);
						file.put(m_pixels[idx + 3]);
					}
				}
				file.close();
				return;
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