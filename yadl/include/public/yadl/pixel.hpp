#pragma once
#include <cstdint>
#include <algorithm>
namespace yadl
{

    struct Pixel
    {         
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        constexpr Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
        constexpr Pixel(uint32_t rgba) : r((rgba >> 0) & 0xFF)
                                       , g((rgba >> 8) & 0xFF)
                                       , b((rgba >> 16) & 0xFF)
                                       , a((rgba >> 24) & 0xFF) {}
        constexpr Pixel() : r(0), g(0), b(0), a(0) {}
        constexpr Pixel(const Pixel &other) = default;
        constexpr Pixel &operator=(const Pixel &other) = default;
        constexpr Pixel(Pixel &&other) noexcept = default;
        constexpr Pixel &operator=(Pixel &&other) noexcept = default;

        constexpr inline bool operator==(const Pixel &other) const
        {
            int32_t self_rgba{a << 24 | b << 16 | g << 8 | r};
            int32_t other_rgba{other.a << 24 | other.b << 16 | other.g << 8 | other.r};
            return self_rgba == other_rgba;
        }

        constexpr inline bool operator!=(const Pixel &other) const
        {
            int32_t self_rgba{a << 24 | b << 16 | g << 8 | r};
            int32_t other_rgba{other.a << 24 | other.b << 16 | other.g << 8 | other.r};
            return self_rgba != other_rgba;
        }

        constexpr inline Pixel &operator+=(const Pixel &other)
        {
            r = static_cast<uint8_t>(std::min(255, static_cast<int32_t>(r) + static_cast<int32_t>(other.r)));
            g = static_cast<uint8_t>(std::min(255, static_cast<int32_t>(g) + static_cast<int32_t>(other.g)));
            b = static_cast<uint8_t>(std::min(255, static_cast<int32_t>(b) + static_cast<int32_t>(other.b)));
            // a = static_cast<uint8_t>(std::min(255, static_cast<int32_t>(a) + static_cast<int32_t>(other.a)));
            return *this;
        }

        constexpr inline Pixel &operator-=(const Pixel &other)
        {
            r = static_cast<uint8_t>(std::max(0, static_cast<int32_t>(r) - static_cast<int32_t>(other.r)));
            g = static_cast<uint8_t>(std::max(0, static_cast<int32_t>(g) - static_cast<int32_t>(other.g)));
            b = static_cast<uint8_t>(std::max(0, static_cast<int32_t>(b) - static_cast<int32_t>(other.b)));
            // a = static_cast<uint8_t>(std::max(0, static_cast<int32_t>(a) - static_cast<int32_t>(other.a)));
            return *this;
        }

        constexpr inline Pixel &operator*=(const Pixel &other)
        {
            r = static_cast<uint8_t>(std::min(255, static_cast<int32_t>(static_cast<float>(r) * static_cast<float>(other.r) / static_cast<float>(255))));
            g = static_cast<uint8_t>(std::min(255, static_cast<int32_t>(static_cast<float>(g) * static_cast<float>(other.g) / static_cast<float>(255))));
            b = static_cast<uint8_t>(std::min(255, static_cast<int32_t>(static_cast<float>(b) * static_cast<float>(other.b) / static_cast<float>(255))));
            // a = static_cast<uint8_t>(std::min(255, static_cast<int32_t>(a) * static_cast<int32_t>(other.a) / 255));
            return *this;
        }

        constexpr inline uint32_t ToInt32() const
        {
            return (a << 24) | (b << 16) | (g << 8) | r;
        }

        constexpr static uint32_t RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            return (a << 24) | (b << 16) | (g << 8) | r;
        }

        /**
         * @brief Linear interpolation between two colors
         * 
         * @warning t must be in range [0, 1], otherwise the result is undefined
         * 
         * @param a first color
         * @param b second color
         * @param t interpolation factor
         * @return constexpr Pixel 
         */
        constexpr static Pixel Lerp(const Pixel &a, const Pixel &b, float t)
        {
            return Pixel(static_cast<uint8_t>(a.r + (b.r - a.r) * t),
                         static_cast<uint8_t>(a.g + (b.g - a.g) * t),
                         static_cast<uint8_t>(a.b + (b.b - a.b) * t),
                         static_cast<uint8_t>(a.a + (b.a - a.a) * t));
        }

        /**
         * @brief Barycentric interpolation between three colors
         * 
         * @warning Sum of u, v, w must be 1, otherwise the result is undefined
         * 
         * @param a first color
         * @param b second color
         * @param c third color
         * @param u 
         * @param v 
         * @param w 
         * @return constexpr Pixel 
         */
        constexpr static Pixel Lerp(const Pixel &a, const Pixel &b, const Pixel &c, float u, float v, float w)
        {
            return Pixel(static_cast<uint8_t>(a.r * u + b.r * v + c.r * w),
                         static_cast<uint8_t>(a.g * u + b.g * v + c.g * w),
                         static_cast<uint8_t>(a.b * u + b.b * v + c.b * w),
                         static_cast<uint8_t>(a.a * u + b.a * v + c.a * w));
        }

        // todo slerp

        constexpr Pixel &Blend(const Pixel &other)
        {
            // https://registry.khronos.org/OpenGL-Refpages/gl4/html/mix.xhtml
            // x * (1 - a) + y * a
            int32_t r1 = static_cast<int32_t>(r);
            int32_t g1 = static_cast<int32_t>(g);
            int32_t b1 = static_cast<int32_t>(b);
            // int32_t a1 = static_cast<int32_t>(a);

            int32_t r2 = static_cast<int32_t>(other.r);
            int32_t g2 = static_cast<int32_t>(other.g);
            int32_t b2 = static_cast<int32_t>(other.b);
            int32_t a2 = static_cast<int32_t>(other.a);

            r = static_cast<uint8_t>(std::min(255, (r1 * (255 - a2) + r2 * a2) / 255));
            g = static_cast<uint8_t>(std::min(255, (g1 * (255 - a2) + g2 * a2) / 255));
            b = static_cast<uint8_t>(std::min(255, (b1 * (255 - a2) + b2 * a2) / 255));

            // alpha is the original one
            // half red + half green = ??
            // ... half yellow :)
            return *this;
        }
    };

    namespace Color
    {
        constexpr Pixel Black = Pixel(0, 0, 0, 255);
        constexpr Pixel White = Pixel(255, 255, 255, 255);
        constexpr Pixel Red = Pixel(255, 0, 0, 255);
        constexpr Pixel Green = Pixel(0, 255, 0, 255);
        constexpr Pixel Blue = Pixel(0, 0, 255, 255);
        constexpr Pixel Yellow = Pixel(255, 255, 0, 255);
        constexpr Pixel Cyan = Pixel(0, 255, 255, 255);
        constexpr Pixel Magenta = Pixel(255, 0, 255, 255);
        constexpr Pixel Transparent = Pixel(0, 0, 0, 0);
        constexpr Pixel Dark = Pixel(0x18, 0x18, 0x18, 255);
    }
}