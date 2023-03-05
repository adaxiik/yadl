#include "ppm.hpp"
#include <fstream>
#include <cstdio>
bool yadl::PPM::Save(const std::string& filename,int32_t width, int32_t height, const uint8_t* pixels, int32_t byteStride)
{
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file.is_open())
        return false;

    file << "P6\n" << std::to_string(width) << " " << std::to_string(height) << "\n255\n";
    
    // TODO: fix this
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