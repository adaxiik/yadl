#include "canvas.hpp"

#include <cstring>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rdparty/stb_image_write.h"

#include "ppm.hpp"

namespace yadl
{
    Canvas::Canvas()
        : m_width{0}
        , m_height{0}
        , m_pixelStride{0}
        , m_pixels{nullptr}
        {}


    Canvas::Canvas(int32_t width, int32_t height)
        : m_width{width}
        , m_height{height}
        , m_pixelStride{width}
        , m_pixels{std::shared_ptr<Pixel[]>(new Pixel[width * height])}
        {}

    Canvas::Canvas(const Canvas& other)
        : m_width{other.m_width}
        , m_height{other.m_height}
        , m_pixelStride{other.m_pixelStride}
        , m_pixels{other.m_pixels}
        {}
    
    Canvas& Canvas::operator=(const Canvas& other)
    {
        m_width = other.m_width;
        m_height = other.m_height;
        m_pixelStride = other.m_pixelStride;
        m_pixels = other.m_pixels;
        return *this;
    }

    Canvas::Canvas(Canvas&& other) noexcept
        : m_width{other.m_width}
        , m_height{other.m_height}
        , m_pixelStride{other.m_pixelStride}
        , m_pixels{other.m_pixels}
        {}

    Canvas& Canvas::operator=(Canvas&& other) noexcept
    {
        m_width = other.m_width;
        m_height = other.m_height;
        m_pixelStride = other.m_pixelStride;
        m_pixels = other.m_pixels;
        return *this;
    }
    



    Canvas::Canvas(const std::string &filename)
    {
        bool success = Load(filename);
        if (!success)
            throw std::runtime_error("Failed to load image");
    }



    bool Canvas::Save(const std::string &filename, FileFormat fileformat) const
    {
        switch (fileformat)
        {
        case FileFormat::PNG:
            stbi_write_png(filename.c_str(), m_width, m_height, 4, m_pixels.get(), m_pixelStride * sizeof(Pixel));
            return true;
            break;
        case FileFormat::PPM:
            PPM::Save(filename, m_width, m_height, GetBytes(), GetByteStride());
            return true;
            break;

        // case FileFormat::JPG:
        //     stbi_write_jpg(filename.c_str(), m_width, m_height, 4, m_pixels.get(), 100);
        //     return true;
        //     break;
        // case FileFormat::BMP:
        //     stbi_write_bmp(filename.c_str(), m_width, m_height, 4, m_pixels.get());
        //     return true;
        //     break;
        // case FileFormat::TGA:
        //     stbi_write_tga(filename.c_str(), m_width, m_height, 4, m_pixels.get());
        //     return true;
        //     break;
    
        default:
            return false;
        }

        return false;
    }

    bool Canvas::Load(const std::string &filename)
    {
        int32_t channels;
        std::unique_ptr<uint8_t[], decltype(&stbi_image_free)> data(stbi_load(filename.c_str(), &m_width, &m_height, &channels, 4), stbi_image_free);

        m_pixelStride = m_width;

        m_pixels = std::make_shared<Pixel[]>(m_width * m_height);
        std::memcpy(m_pixels.get(), data.get(), m_width * m_height * sizeof(Pixel));

        return true;
    }
    void Canvas::Clear(Pixel pixel)
    {
        for (int32_t y = 0; y < m_height; y++)
        {
            for (int32_t x = 0; x < m_width; x++)
            {
                SetPixel(x, y, pixel);
            }
        }
    }

    Canvas Canvas::SubCanvas(int32_t x, int32_t y, int32_t width, int32_t height) const
    {
        x = std::clamp(x, 0, m_width);
        y = std::clamp(y, 0, m_height);
        width = std::clamp(width, 0, m_width - x);
        height = std::clamp(height, 0, m_height - y);

        Canvas c(width, height);
        
        c.m_pixelStride = m_pixelStride;
        // https://stackoverflow.com/questions/54737556/set-shared-ptr-with-new-pointer-that-is-old-pointer-offset
        c.m_pixels = std::shared_ptr<Pixel[]>(m_pixels, m_pixels.get() + (y * m_pixelStride + x));

        return c;
    }

    Canvas::~Canvas()
    {
    }
}