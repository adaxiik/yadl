#include "canvas.hpp"

#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "io.hpp"

#ifdef YADL_DEBUG
    #ifndef YADL_LOGGER_SETTINGS_LEVEL
        #define YADL_LOGGER_SETTINGS_LEVEL YADL_LOGGER_LEVEL_INFO
    #endif
    #include "debug.hpp"
#endif

namespace yadl
{
    Canvas::Canvas(): Canvas(0, 0) {}


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
        // io shouldn't be part of this class, but its pretty convenient to have it here, so I'll leave it for now c:
        auto canvas = io::Load(filename);
        if(!canvas.has_value())
            throw std::runtime_error("Failed to load image: " + filename);

        *this = canvas.value();
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

    Canvas& Canvas::Clear(Pixel pixel)
    {
        for (int32_t y = 0; y < m_height; y++)
            for (int32_t x = 0; x < m_width; x++)
                RefPixel(x, y) = pixel;

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

    Canvas Canvas::DeepCopy() const
    {
        Canvas c(m_width, m_height);
        for (int32_t y = 0; y < m_height; y++)
            for (int32_t x = 0; x < m_width; x++)
                c.RefPixel(x, y) = GetPixel(x, y);
        return c;
    }

    bool Canvas::IsDeepEqual(const Canvas& other) const
    {
        if (!IsSameSize(other))
            return false;

        for (int32_t y = 0; y < m_height; y++)
            for (int32_t x = 0; x < m_width; x++)
                if (GetPixel(x, y) != other.GetPixel(x, y))
                    return false;

        return true;
    }

    Canvas Canvas::Resize(int32_t width, int32_t height) const
    {
        if(width == m_width && height == m_height)
            return DeepCopy();

        Canvas c(width, height);
        for (int32_t y = 0; y < height; y++)
            for (int32_t x = 0; x < width; x++)
                c.RefPixel(x, y) = GetPixel(static_cast<int32_t>(static_cast<float>(x) / width * m_width)
                                          , static_cast<int32_t>(static_cast<float>(y) / height * m_height));
        return c;
    }

}