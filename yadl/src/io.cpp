#include "io.hpp"
#include <fstream>
#include <cstdio>
#include <cstring>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rdparty/stb_image_write.h"

#include "3rdparty/gif.h"

namespace yadl
{
    namespace io
    {
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

        bool SaveAsGIF(const std::string &filename, const Animation &animation, int32_t delay)
        {
            GifWriter writer;
            GifBegin(&writer, filename.c_str(), animation.GetWidth(), animation.GetHeight(), delay);
            std::unique_ptr<uint8_t[]> buffer_to_use_cpp_safely_smiley_face(new uint8_t[animation.GetWidth() * animation.GetHeight() * 4]);
            
            for (size_t i = 0; i < animation.GetFrameCount(); i++)
            {
                std::memcpy(buffer_to_use_cpp_safely_smiley_face.get(), animation[i].GetBytes(), animation.GetWidth() * animation.GetHeight() * 4);
                GifWriteFrame(&writer, buffer_to_use_cpp_safely_smiley_face.get(), animation.GetWidth(), animation.GetHeight(), delay);
            }
            
            GifEnd(&writer);
            return true;
        }


        bool SaveAsY4M(const std::string& filename, const Animation& animation, uint32_t fps)
        {
            std::ofstream file(filename, std::ios::out | std::ios::binary);
            if (!file.is_open())
                return false;

            file << "YUV4MPEG2 W" << animation.GetWidth() << " H" << animation.GetHeight() << " F" << fps << ":1 Ip A1:1 C444\n";

            // https://sistenix.com/rgb2ycbcr.html
            for (size_t i = 0; i < animation.GetFrameCount(); i++)
            {
                file << "FRAME\n";
                for (int32_t y = 0; y < animation.GetHeight(); y++)
                {
                    for (int32_t x = 0; x < animation.GetWidth(); x++)
                    {
                        Pixel p = animation[i].GetPixel(x, y);
                        float r = p.r / 256.0f;
                        float g = p.g / 256.0f;
                        float b = p.b / 256.0f;
                        uint8_t y = static_cast<uint8_t>(16 + 65.738f * r + 129.057f * g + 25.064f * b);
                        file << y;
                    }
                }
                for (int32_t y = 0; y < animation.GetHeight(); y++)
                {
                    for (int32_t x = 0; x < animation.GetWidth(); x++)
                    {
                        Pixel p = animation[i].GetPixel(x, y);
                        float r = p.r / 256.0f;
                        float g = p.g / 256.0f;
                        float b = p.b / 256.0f;
                        uint8_t cb = static_cast<uint8_t>(128 - 37.945f * r - 74.494f * g + 112.439f * b);
                        file << cb;
                    }
                }
                for (int32_t y = 0; y < animation.GetHeight(); y++)
                {
                    for (int32_t x = 0; x < animation.GetWidth(); x++)
                    {
                        Pixel p = animation[i].GetPixel(x, y);
                        float r = p.r / 256.0f;
                        float g = p.g / 256.0f;
                        float b = p.b / 256.0f;
                        uint8_t cr = static_cast<uint8_t>(128 + 112.439f * r - 94.154f * g - 18.285f * b);
                        file << cr;
                    }
                }
            }

            file.close();
            return true;
        }
    }
}