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
        /**
         * @brief Construct a new Canvas object
         * 
         * @note This will create a canvas with 0 width and 0 height.
         * 
         */
        Canvas();

        /**
         * @brief Construct a new Canvas object
         * 
         * @param width 
         * @param height 
         */
        Canvas(int32_t width, int32_t height);

        /**
         * @brief Construct a new Canvas and load from file
         * 
         * @note This will throw an exception if the file cannot be loaded, not recommended to use without try-catch.
         * 
         * @param filename 
         */
        Canvas(const std::string& filename);
        Canvas(const Canvas& other);
        Canvas& operator=(const Canvas& other);
        Canvas(Canvas&& other) noexcept;
        Canvas& operator=(Canvas&& other) noexcept;
        ~Canvas();

        /**
         * @brief Get the Width of the canvas
         * 
         * @return int32_t 
         */
        inline int32_t GetWidth()       const { return m_width; }

        /**
         * @brief Get the Height of the canvas
         * 
         * @return int32_t 
         */
        inline int32_t GetHeight()      const { return m_height; }

        /**
         * @brief Get the Byte Stride of the canvas
         * 
         * @details Byte stride is the number of bytes between the start of one row of pixels and the start of the next row.
         * @sa GetPixelStride()
         * @return int32_t sizeof(Pixel) * PixelStride
         */
        inline int32_t GetByteStride()  const { return m_pixelStride * sizeof(Pixel); }
        
        /**
         * @brief Get the Pixel Stride of the canvas
         * 
         * @details Pixel stride is the number of pixels between the start of one row of pixels and the start of the next row.
         * @sa GetByteStride()
         * 
         * @return int32_t 
         */
        inline int32_t GetPixelStride() const { return m_pixelStride; }

        /**
         * @brief Get the Pixel Count of the canvas
         * 
         * @return int32_t Width * Height
         */
        inline int32_t GetPixelCount() const { return m_width * m_height; }

        /**
         * @brief Get the Center of the canvas on X axis
         * 
         * @sa GetCenterY()
         * 
         * @return int32_t Width / 2
         */
        inline int32_t GetCenterX() const { return m_width / 2; }

        /**
         * @brief Get the Center of the canvas on Y axis
         * 
         * @sa GetCenterX()
         * 
         * @return int32_t Height / 2
         */
        inline int32_t GetCenterY() const { return m_height / 2; }

        /**
         * @brief Get the Pixel at the specified position
         * 
         * @note If the yadl is compiled with YADL_SAFE_PIXEL_ACCESS, x and y will be clamped to the canvas size, otherwise it will be undefined behavior.
         * 
         * @sa SetPixel()
         * @sa RefPixel()
         * 
         * @param x 
         * @param y 
         * @return Pixel
         */
        Pixel GetPixel(int32_t x, int32_t y) const;

        /**
         * @brief Set the Pixel at the specified position
         * 
         * @note If the yadl is compiled with YADL_SAFE_PIXEL_ACCESS, x and y will be clamped to the canvas size, otherwise it will be undefined behavior.
         * 
         * @sa GetPixel()
         * @sa RefPixel()
         * 
         * @param x 
         * @param y 
         * @param pixel 
         */
        void SetPixel(int32_t x, int32_t y, Pixel pixel);

        /**
         * @brief Get a reference to the Pixel at the specified position
         * 
         * @note If the yadl is compiled with YADL_SAFE_PIXEL_ACCESS, x and y will be clamped to the canvas size, otherwise it will be undefined behavior.
         * 
         * @sa GetPixel()
         * @sa SetPixel()
         * 
         * @param x 
         * @param y 
         * @return Pixel& 
         */
        Pixel& RefPixel(int32_t x, int32_t y);

        /**
         * @brief Get the Raw Pixel array
         * 
         * @note Not recommended to modify the array directly, use GetPixel(), SetPixel() and RefPixel() instead.
         * 
         * @sa GetPixel()
         * @sa SetPixel()
         * @sa RefPixel()
         * 
         * @return const Pixel* 
         */
        inline const Pixel* GetBytes() const { return m_pixels.get(); }

        /**
         * @brief Clear the canvas with the specified color
         * 
         * @param pixel color 
         * @return Canvas& 
         */
        Canvas& Clear(Pixel pixel);

        /**
         * @brief Returns a sub canvas of the current canvas
         * 
         * @note Sub canvas will share the same pixel array with the current canvas, so modifying the sub canvas will also modify the current canvas.
         * @note Sub canvas will be clamped to the current canvas size.
         * 
         * @param x 
         * @param y 
         * @param width 
         * @param height 
         * @return Canvas 
         */
        Canvas SubCanvas(int32_t x, int32_t y, int32_t width, int32_t height) const;

        /**
         * @brief Return a new canvas with the same size and pixel array as the current canvas
         * 
         * @note Modifications to the new canvas will not affect the current canvas.
         * 
         * @return Canvas 
         */
        Canvas DeepCopy() const;

        /**
         * @brief Compare the size of the canvas with another canvas
         * 
         * @param other 
         * @return true if the size of the canvas is the same as the other canvas
         * @return false otherwise
         */
        bool IsSameSize(const Canvas& other) const { return m_width == other.m_width && m_height == other.m_height; }
        
        /**
         * @brief Compare canvas with another canvas pixel by pixel
         * 
         * @param other 
         * @return true if the size and pixel array of the canvas is the same as the other canvas
         * @return false otherwise
         */
        bool IsDeepEqual(const Canvas& other) const;

        /**
         * @brief Returns a new canvas with the specified size
         * 
         * @note Modifications to the new canvas will not affect the current canvas.
         * 
         * @param width 
         * @param height 
         * @return Canvas resized deep copy of the current canvas 
         */
        Canvas Resize(int32_t width, int32_t height) const;

        inline Canvas Resize(float widthFactor, float heightFactor) const 
        { 
            return Resize(static_cast<int32_t>(m_width * widthFactor), static_cast<int32_t>(m_height * heightFactor));
        }
        inline Canvas Resize(int32_t size) const { return Resize(size, size); }
        inline Canvas Resize(float factor) const { return Resize(factor, factor); }

        /**
         * @brief Paste the other canvas on the current canvas starting at the specified position (top-left)
         * 
         * @param other canvas that will be pasted on the current canvas
         * @param x position in the current canvas
         * @param y position in the current canvas
         * @return Canvas& 
         */
        Canvas& Paste(const Canvas& other, int32_t x, int32_t y);

    };
}
