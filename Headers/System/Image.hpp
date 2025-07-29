#ifndef _SYSTEM_IMAGE_H
#define _SYSTEM_IMAGE_H


#include "System/Drawing/Size.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "Security/Cryptography/CRC32.h"
#include "System/IO/BinaryReader.h"
#include "System/IO/Compression/Deflate.hpp"
#include <stdexcept>
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
#pragma pack(push, 1)
			struct PNG_IHDR {
				uint32_t width;
				uint32_t height;
				uint8_t bitDepth;
				uint8_t colorType;
				uint8_t compression;
				uint8_t filter;
				uint8_t interlace;
				void fixendian() {
#if defined(_MSC_VER)
					width = _byteswap_ulong(width);
					height = _byteswap_ulong(height);
#else
					width = __builtin_bswap32(width);
					height = __builtin_bswap32(height);
#endif
				}
			};
#pragma pack(pop)
			static uint32_t ReadUInt32BigEndian(std::ifstream& stream) {
				uint32_t value = 0;
				uint8_t bytes[4];
				stream.read(reinterpret_cast<char*>(bytes), 4);
				value = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
				return value;
			}

			uint8_t PaethPredictor(uint8_t a, uint8_t b, uint8_t c) {
				int p = a + b - c;
				int pa = std::abs(p - a);
				int pb = std::abs(p - b);
				int pc = std::abs(p - c);
				if (pa <= pb && pa <= pc) return a;
				if (pb <= pc) return b;
				return c;
			}

			void Unfilter(const std::vector<uint8_t>& data, PNG_IHDR& ihdr) {
				int bytesPerPixel = 0;
				if (ihdr.colorType == 6) { // RGBA
					bytesPerPixel = 4;
				}
				else if (ihdr.colorType == 2) { // RGB
					bytesPerPixel = 3;
				}
				else {
					// Other color types not supported yet
					return;
				}

				int stride = ihdr.width * bytesPerPixel;
				m_pixels.resize(stride * ihdr.height);

				for (int y = 0; y < ihdr.height; ++y) {
					uint8_t filterType = data[y * (stride + 1)];
					int scanlineStart = y * (stride + 1) + 1;

					for (int x = 0; x < stride; ++x) {
						uint8_t filteredByte = data[scanlineStart + x];
						uint8_t unfilteredByte = 0;

						uint8_t a = (x >= bytesPerPixel) ? m_pixels[y * stride + x - bytesPerPixel] : 0;
						uint8_t b = (y > 0) ? m_pixels[(y - 1) * stride + x] : 0;
						uint8_t c = (x >= bytesPerPixel && y > 0) ? m_pixels[(y - 1) * stride + x - bytesPerPixel] : 0;

						switch (filterType) {
						case 0: // None
							unfilteredByte = filteredByte;
							break;
						case 1: // Sub
							unfilteredByte = filteredByte + a;
							break;
						case 2: // Up
							unfilteredByte = filteredByte + b;
							break;
						case 3: // Average
							unfilteredByte = filteredByte + (a + b) / 2;
							break;
						case 4: // Paeth
							unfilteredByte = filteredByte + PaethPredictor(a, b, c);
							break;
						default:
							// Unknown filter type
							break;
						}
						m_pixels[y * stride + x] = unfilteredByte;
					}
				}
			}
        public:
            union {
                struct {
                    int32_t Width;
                    int32_t Height;
                };
                System::Drawing::Size Size;
            };

            std::vector<uint8_t> m_pixels;
			Image(int w, int h);
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
				inline uint32_t GetType() const {
#if defined(_MSC_VER)
					return _byteswap_ulong(Type);
#else
					return __builtin_bswap32(Type);
#endif
				}
			};
			void LoadFromPNG(const std::string& filename);
			void SaveAsPNG(const std::string& filename);
			void SaveAsBitmap(const std::string& filename);
			void SaveAsTGA(const std::string& filename);
			void LoadFromBitmap(const std::string& filename);
			void LoadFromTGA(const std::string& filename);
			void Flip();
			~Image();
        };
    }
#endif