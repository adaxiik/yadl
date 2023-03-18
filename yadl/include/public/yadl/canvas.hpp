#pragma once

#include <memory>
#include <cstdint>

#include "pixel.hpp"

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

        inline int32_t GetWidth()       const { return m_width; }
        inline int32_t GetHeight()      const { return m_height; }
        inline int32_t GetByteStride()  const { return m_pixelStride * sizeof(Pixel); }
        inline int32_t GetPixelStride() const { return m_pixelStride; }
        inline int32_t GetPixelCount()  const { return m_width * m_height; }
        inline int32_t GetCenterX()     const { return m_width / 2; }
        inline int32_t GetCenterY()     const { return m_height / 2; }

        Pixel GetPixel(int32_t x, int32_t y) const ;
        void SetPixel(int32_t x, int32_t y, Pixel pixel);
        Pixel& RefPixel(int32_t x, int32_t y);

        inline const Pixel* GetBytes() const { return m_pixels.get(); }

        Canvas& Clear(Pixel pixel);
        Canvas SubCanvas(int32_t x, int32_t y, int32_t width, int32_t height) const;
        Canvas DeepCopy() const;
        bool IsSameSize(const Canvas& other) const { return m_width == other.m_width && m_height == other.m_height; }
        bool IsDeepEqual(const Canvas& other) const;
        Canvas Resize(int32_t width, int32_t height) const;
        inline Canvas Resize(float widthFactor, float heightFactor) const 
        { 
            return Resize(static_cast<int32_t>(m_width * widthFactor), static_cast<int32_t>(m_height * heightFactor));
        }
        inline Canvas Resize(int32_t size) const { return Resize(size, size); }
        inline Canvas Resize(float factor) const { return Resize(factor, factor); }
    };
}
