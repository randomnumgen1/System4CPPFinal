#ifndef _SYSTEM_IMAGE_H
#define _SYSTEM_IMAGE_H


#include "System/Drawing/Size.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "Security/Cryptography/CRC32.h"

namespace System {

        class Image {
		private:
			static constexpr uint16_t BITMAP_MAGIC = 0x4D42;
			
#pragma pack(push, 1)
			struct BITMAPFILEHEADER {
				uint16_t Magic;      // File type; must be 'BM' (0x4D42)
				uint32_t FileSize;      // Size of the file in bytes
				uint16_t Reserved1; // Reserved; must be 0
				uint16_t Reserved2; // Reserved; must be 0
				uint32_t pixeloffset;   // Offset to pixel data from start of file
			};
#pragma pack(pop)
#pragma pack(push, 1)
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
#pragma pack(push, 1)
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
				TGA,
				PNG
			};
			void Load(const std::string& filename) {
				std::string ext = filename.substr(filename.find_last_of(".") + 1);
				if (ext == "bmp") {
					LoadFromBitmap(filename);
				}else if (ext == "tga") {
					LoadFromTGA(filename);
				}else{
				
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
				default:
					SaveAsPNG(filename);
					break;
				}
			}
			static constexpr uint64_t PNG_MAGIC = 0x89504E470D0A1A0A;
			struct PNG_CHUNK {
				uint32_t Length;
				uint32_t Type;
				std::vector<uint8_t> Data;
				uint32_t CRC;
			};
			struct PNG_IHDR {
				uint32_t width;
				uint32_t height;
				uint8_t bitDepth;
				uint8_t colorType;
				uint8_t compression;
				uint8_t filter;
				uint8_t interlace;
			};

			void SaveAsPNG(const std::string& filename){
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
			void LoadFromTGA(const std::string& filename) {
				std::ifstream file(filename, std::ios::binary);
				if (!file) return;
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
					}else{
						throw std::runtime_error("unsupported tga image bpp");
					}
				}else{
					throw std::runtime_error("unsupported tga image type");
				}
			}







			

            ~Image() {
                // Clear vector and ensure raw memory is managed correctly
                m_pixels.clear();
            }
        };
    }
#endif