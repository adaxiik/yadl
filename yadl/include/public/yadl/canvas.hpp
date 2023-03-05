#pragma once

#include <memory>
#include <cstdint>

#include "pixel.hpp"
#include "fileformat.hpp"

namespace yadl
{
    class Canvas
    {
    private:
        int32_t m_width;
        int32_t m_height;
        int32_t m_pixelStride;
        std::shared_ptr<Pixel[]> m_pixels;

    public:
        Canvas();
        Canvas(int32_t width, int32_t height);
        Canvas(const std::string& filename);
        Canvas(const Canvas& other);
        Canvas& operator=(const Canvas& other);
        Canvas(Canvas&& other) noexcept;
        Canvas& operator=(Canvas&& other) noexcept;
        ~Canvas();

        bool Load(const std::string& filename);
        bool Save(const std::string& filename, FileFormat fileformat) const;
        inline int32_t GetWidth() const { return m_width; }
        inline int32_t GetHeight() const { return m_height; }
        inline int32_t GetByteStride() const { return m_pixelStride * sizeof(Pixel); }
        inline int32_t GetPixelStride() const { return m_pixelStride; }

        inline Pixel GetPixel(int32_t x, int32_t y) const { return m_pixels[y  * m_pixelStride + x]; }

        inline void SetPixel(int32_t x, int32_t y, Pixel pixel) { m_pixels[y  * m_pixelStride + x] = pixel; }

        inline uint8_t* GetBytes() const { return reinterpret_cast<uint8_t*>(m_pixels.get()); }

        void Clear(Pixel pixel);
        Canvas SubCanvas(int32_t x, int32_t y, int32_t width, int32_t height) const;

    };
}
