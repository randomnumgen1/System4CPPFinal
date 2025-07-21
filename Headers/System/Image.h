#ifndef _SYSTEM_IMAGE_H
#define _SYSTEM_IMAGE_H


#include "System/Drawing/Size.h"
#include <vector>
#include <iostream>
#include <fstream>

namespace System {

        struct Image {
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

				return;
			}
















  
            void FromFile(const std::string& file) {
                FromFile(file.c_str());
            }

            ~Image() {
                // Clear vector and ensure raw memory is managed correctly
                m_pixels.clear();
            }
        };
    }
#endif