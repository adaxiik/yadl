#pragma once
#include <string>
#include <memory>
#include "font.hpp"
#include "pixel.hpp"
#include "canvas.hpp"
namespace yadl
{
    class Text
    {
    private:
        Text() = default;
        Text(const Text &) = delete;
        Text &operator=(const Text &) = delete;
        Text(Text &&) = delete;
        Text &operator=(Text &&) = delete;
        ~Text() = default;

        Font m_font{};
        int32_t m_positionX{0};
        int32_t m_positionY{0};
        Pixel m_color{0, 0, 0, 255};
        float m_scale{1.0f};
        Canvas m_canvas;
    public:
        inline static Text &Get()
        {
            static Text instance;
            return instance;
        }

        inline Text &SetFont(const Font &font)
        {
            m_font = font;
            return *this;
        }

        inline Text &SetPosition(int32_t x, int32_t y)
        {
            m_positionX = x;
            m_positionY = y;
            return *this;
        }

        inline Text &SetColor(Pixel color)
        {
            m_color = color;
            return *this;
        }

        inline Text &SetScale(float scale)
        {
            m_scale = scale;
            return *this;
        }

        inline Text &SetCanvas(Canvas canvas)
        {
            m_canvas = canvas;
            return *this;
        }

        inline int32_t GetCurrentTextHeight() const
        {
            return 42 * m_scale; // magic constant :) 
            // TODO: replace 42, with less magic constant 
            // font is loaded with 42 as height
            // @see font_internal.cpp
        }

        Text &DrawText(const std::string &text);
    };
}