#pragma once
#include "stb_image.h"
/*
Image.h
*/
#include "Size.h"
#include <vector>
#include <iostream>
#include <fstream>

namespace System {
    namespace Drawing { 

        struct Image {
        public:
            union {
                struct {
                    int32_t Width;
                    int32_t Height;
                };
                System::Drawing::Size Size;
            };
            std::vector<uint8_t> pixels;

          

            void FromFile(const char* filepath) {
                int channels;
                unsigned char* rawPixels = stbi_load(filepath, &Width, &Height, &channels, 0);
                if (rawPixels) {
                    // Store the data in pixels vector
                    pixels.assign(rawPixels, rawPixels + (Width * Height * channels));

                    // Log details
                    std::cout << "Image loaded successfully!" << std::endl;
                    std::cout << "Width: " << Width << ", Height: " << Height << ", Channels: " << channels << std::endl;

                    // Free raw memory allocated by stb_image
                    stbi_image_free(rawPixels);
                }
                else {
                    std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
                }
            }

            void FromFile(const std::string& file) {
                FromFile(file.c_str());
            }

            ~Image() {
                // Clear vector and ensure raw memory is managed correctly
                pixels.clear();
            }
        };
    }
}
