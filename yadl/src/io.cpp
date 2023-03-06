#include "io.hpp"
#include <fstream>
#include <cstdio>
#include <cstring>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rdparty/stb_image_write.h"

namespace yadl
{
    namespace io
    {
        bool SaveAsPPM6(const std::string &filename, int32_t width, int32_t height, const uint8_t *pixels, int32_t byteStride)
        {
            std::ofstream file(filename, std::ios::out | std::ios::binary);
            if (!file.is_open())
                return false;

            file << "P6\n"
                 << std::to_string(width) << " " << std::to_string(height) << "\n255\n";

            for (int32_t y = 0; y < height; y++)
            {
                for (int32_t x = 0; x < width; x++)
                {
                    file << pixels[y * byteStride + x * 4 + 0]; // R
                    file << pixels[y * byteStride + x * 4 + 1]; // G
                    file << pixels[y * byteStride + x * 4 + 2]; // B
                }
            }

            file.close();
            return true;
        }

        bool SaveAsPNG(const std::string &filename, int32_t width, int32_t height, const uint8_t *pixels, int32_t byteStride)
        {
            return stbi_write_png(filename.c_str(), width, height, 4, pixels, byteStride) != 0;
        }

        std::shared_ptr<Pixel[]> Load(const std::string &filename, int32_t &width, int32_t &height)
        {
            int32_t channels;
            std::unique_ptr<uint8_t[], decltype(&stbi_image_free)> data(stbi_load(filename.c_str(), &width, &height, &channels, 4), stbi_image_free);
            if (!data)
                return nullptr;

            std::shared_ptr<Pixel[]> pixels{new Pixel[width * height]};
            std::memcpy(pixels.get(), data.get(), width * height * sizeof(Pixel));
            
            return pixels;
        }
    }
}