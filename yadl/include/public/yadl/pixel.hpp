#pragma once
#include <cstdint>
namespace yadl
{

    struct Pixel
    {
        union
        {
            struct
            {
                uint8_t r;
                uint8_t g;
                uint8_t b;
                uint8_t a;
            };
            uint32_t rgba;
        };

        constexpr Pixel( uint8_t r, uint8_t g, uint8_t b, uint8_t a ): r(r), g(g), b(b), a(a) {}
        constexpr Pixel( uint32_t rgba ) : rgba(rgba) {}
        constexpr Pixel(): r(0), g(0), b(0), a(0) {}
        constexpr Pixel(const Pixel& other) = default;
        constexpr Pixel& operator=(const Pixel& other) = default;
        constexpr Pixel(Pixel&& other) noexcept = default;
        constexpr Pixel& operator=(Pixel&& other) noexcept = default;


        
        inline bool operator==(const Pixel& other) const
        {
            return rgba == other.rgba;
        }

        inline bool operator!=(const Pixel& other) const
        {
            return rgba != other.rgba;
        }

        constexpr static uint32_t RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            return (a << 24) | (b << 16) | (g << 8) | r;
        }
        
    };
}
