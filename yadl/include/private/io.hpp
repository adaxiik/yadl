#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include "pixel.hpp"
namespace yadl
{
    namespace io
    {
        bool SaveAsPNG(const std::string& filename,int32_t width, int32_t height, const uint8_t* pixels, int32_t byteStride);
        bool SaveAsPPM6(const std::string& filename,int32_t width, int32_t height, const uint8_t* pixels, int32_t byteStride);
        bool SaveAsPPM3(const std::string& filename,int32_t width, int32_t height, const uint8_t* pixels, int32_t byteStride);
        std::shared_ptr<Pixel[]> Load(const std::string& filename, int32_t& width, int32_t& height);
    }
}