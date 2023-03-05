#pragma once
#include <cstdint>
#include <string>

namespace yadl
{
    namespace PPM
    {
        bool Save(const std::string& filename,int32_t width, int32_t height, const uint8_t* pixels, int32_t byteStride);
    }
}