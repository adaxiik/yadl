#pragma once
#include <cstdint>
#include <algorithm>
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

        constexpr Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
        constexpr Pixel(uint32_t rgba) : rgba(rgba) {}
        constexpr Pixel() : r(0), g(0), b(0), a(0) {}
        constexpr Pixel(const Pixel &other) = default;
        constexpr Pixel &operator=(const Pixel &other) = default;
        constexpr Pixel(Pixel &&other) noexcept = default;
        constexpr Pixel &operator=(Pixel &&other) noexcept = default;

        constexpr inline bool operator==(const Pixel &other) const
        {
            return rgba == other.rgba;
        }

        constexpr inline bool operator!=(const Pixel &other) const
        {
            return rgba != other.rgba;
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

        constexpr static uint32_t RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            return (a << 24) | (b << 16) | (g << 8) | r;
        }

        constexpr Pixel &Blend(const Pixel &other)
        {
            // https://registry.khronos.org/OpenGL-Refpages/gl4/html/mix.xhtml
            // x * (1 - a) + y * a
            int32_t r1 = static_cast<int32_t>(r);
            int32_t g1 = static_cast<int32_t>(g);
            int32_t b1 = static_cast<int32_t>(b);
            int32_t a1 = static_cast<int32_t>(a);

            int32_t r2 = static_cast<int32_t>(other.r);
            int32_t g2 = static_cast<int32_t>(other.g);
            int32_t b2 = static_cast<int32_t>(other.b);

            r = static_cast<uint8_t>(std::min(255, (r1 * (255 - a1) + r2 * a1) / 255));
            g = static_cast<uint8_t>(std::min(255, (g1 * (255 - a1) + g2 * a1) / 255));
            b = static_cast<uint8_t>(std::min(255, (b1 * (255 - a1) + b2 * a1) / 255));

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