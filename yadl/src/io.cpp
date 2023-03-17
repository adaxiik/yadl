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
        // bool SaveAsPPM3(const std::string &filename, int32_t width, int32_t height, const uint8_t *pixels, int32_t byteStride)
        // {
        //
        // }

        // bool SaveAsPNG(const std::string &filename, int32_t width, int32_t height, const uint8_t *pixels, int32_t byteStride)
        // {
        //     return stbi_write_png(filename.c_str(), width, height, 4, pixels, byteStride) != 0;
        // }

        // std::shared_ptr<Pixel[]> Load(const std::string &filename, int32_t &width, int32_t &height)
        // {
        //     int32_t channels;
        //     std::unique_ptr<uint8_t[], decltype(&stbi_image_free)> data(stbi_load(filename.c_str(), &width, &height, &channels, 4), stbi_image_free);
        //     if (!data)
        //         return nullptr;

        //     std::shared_ptr<Pixel[]> pixels{new Pixel[width * height]};
        //     std::memcpy(pixels.get(), data.get(), width * height * sizeof(Pixel));

        //     return pixels;
        // }

        std::optional<Canvas> Load(const std::string &filename)
        {
            int32_t width, height, channels;
            std::unique_ptr<uint8_t[], decltype(&stbi_image_free)> data(stbi_load(filename.c_str(), &width, &height, &channels, 4), stbi_image_free);

            if (!data)
                return std::nullopt;

            Canvas canvas(width, height);
            for (int32_t y = 0; y < height; y++)
            {
                for (int32_t x = 0; x < width; x++)
                {
                    canvas.SetPixel(x, y, Pixel{static_cast<uint8_t>(data[y * width * 4 + x * 4 + 0]), static_cast<uint8_t>(data[y * width * 4 + x * 4 + 1]), static_cast<uint8_t>(data[y * width * 4 + x * 4 + 2]), static_cast<uint8_t>(data[y * width * 4 + x * 4 + 3])});
                }
            }

            return canvas;
        }

        bool SaveAsPNG(const std::string &filename, const Canvas &canvas)
        {
            // TODOOOO
            // Pixel* to void* .. UB? 
            return stbi_write_png(filename.c_str(), canvas.GetWidth(), canvas.GetHeight(), 4, canvas.GetBytes(), canvas.GetByteStride()) != 0;
        }

        bool SaveAsPPM6(const std::string &filename, const Canvas &canvas)
        {
            std::ofstream file(filename, std::ios::out | std::ios::binary);
            if (!file.is_open())
                return false;

            file << "P6\n"
                 << std::to_string(canvas.GetWidth()) << " " << std::to_string(canvas.GetHeight())
                 << "\n255\n";

            for (int32_t y = 0; y < canvas.GetHeight(); y++)
            {
                for (int32_t x = 0; x < canvas.GetWidth(); x++)
                {
                    Pixel pixel = canvas.GetPixel(x, y);
                    file << static_cast<uint8_t>(pixel.r);
                    file << static_cast<uint8_t>(pixel.g);
                    file << static_cast<uint8_t>(pixel.b);
                }
            }

            file.close();
            return true;
        }
        
        bool SaveAsPPM3(const std::string &filename, const Canvas &canvas)
        {
            std::ofstream file(filename);
            if (!file.is_open())
                return false;

            file << "P3\n"
                 << std::to_string(canvas.GetWidth()) << " " << std::to_string(canvas.GetHeight())
                 << "\n255\n";

            for (int32_t y = 0; y < canvas.GetHeight(); y++)
            {
                for (int32_t x = 0; x < canvas.GetWidth(); x++)
                {
                    Pixel pixel = canvas.GetPixel(x, y);
                    file << std::to_string(static_cast<uint8_t>(pixel.r)) << " ";
                    file << std::to_string(static_cast<uint8_t>(pixel.g)) << " ";
                    file << std::to_string(static_cast<uint8_t>(pixel.b)) << " ";
                }
                file << "\n";
            }

            file.close();
            return true;
        }
    }
}