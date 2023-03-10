#include "canvas.hpp"

#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "io.hpp"

#ifdef YADL_DEBUG
    #define YADL_LOGGER_SETTINGS_LEVEL YADL_LOGGER_LEVEL_INFO
    #include "debug.hpp"
#endif

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

    Pixel Canvas::GetPixel(int32_t x, int32_t y) const
    {
        #ifdef YADL_SAFE_PIXEL_ACCESS
            if (x < 0 || x >= m_width || y < 0 || y >= m_height)
            {
                #ifdef YADL_DEBUG
                    YADL_LOG_DEBUG("Pixel out of bounds: (%d, %d)" , x, y);
                #endif
                x = std::clamp(x, 0, m_width - 1);
                y = std::clamp(y, 0, m_height - 1);
            }
        #endif
        return m_pixels[y  * m_pixelStride + x];
    }

    void Canvas::SetPixel(int32_t x, int32_t y, Pixel pixel)
    {
        #ifdef YADL_SAFE_PIXEL_ACCESS
            if (x < 0 || x >= m_width || y < 0 || y >= m_height)
            {
                #ifdef YADL_DEBUG                           
                    YADL_LOG_DEBUG("Pixel out of bounds: (%d, %d)" , x, y);
                #endif
                x = std::clamp(x, 0, m_width - 1);
                y = std::clamp(y, 0, m_height - 1);
            }
        #endif
        m_pixels[y * m_pixelStride + x] = pixel;
    }

    Pixel& Canvas::RefPixel(int32_t x, int32_t y)
    {
        #ifdef YADL_SAFE_PIXEL_ACCESS
            if (x < 0 || x >= m_width || y < 0 || y >= m_height)
            {
                #ifdef YADL_DEBUG                        
                    YADL_LOG_DEBUG("Pixel out of bounds: (%d, %d)" , x, y);
                #endif
                x = std::clamp(x, 0, m_width - 1);
                y = std::clamp(y, 0, m_height - 1);
            }
        #endif
        return m_pixels[y * m_pixelStride + x];
    }

    



    bool Canvas::Save(const std::string &filename, FileFormat fileformat) const
    {
        switch (fileformat)
        {
        case FileFormat::PNG:
            io::SaveAsPNG(filename, m_width, m_height, GetBytes(), GetByteStride());
            return true;
            break;
        case FileFormat::PPM6:
            io::SaveAsPPM6(filename, m_width, m_height, GetBytes(), GetByteStride());
            return true;
            break;
        case FileFormat::PPM3:
            // io::SaveAsPPM3(filename, m_width, m_height, GetBytes(), GetByteStride());
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

        m_pixels = io::Load(filename, m_width, m_height);
        if (m_pixels == nullptr)
            return false;
        m_pixelStride = m_width;

        return true;
    }
    
    Canvas& Canvas::Clear(Pixel pixel)
    {
        for (int32_t y = 0; y < m_height; y++)
            for (int32_t x = 0; x < m_width; x++)
                SetPixel(x, y, pixel);

        return *this;
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

    Canvas Canvas::DeepCopy()
    {
        Canvas c(m_width, m_height);
        for (int32_t y = 0; y < m_height; y++)
            for (int32_t x = 0; x < m_width; x++)
                c.SetPixel(x, y, GetPixel(x, y));
        return c;
    }

    Canvas Canvas::Resize(int32_t width, int32_t height) const
    {
        Canvas c(width, height);
        for (int32_t y = 0; y < height; y++)
            for (int32_t x = 0; x < width; x++)
                c.SetPixel(x, y, GetPixel(x * m_width / width, y * m_height / height));
        return c;
    }


}