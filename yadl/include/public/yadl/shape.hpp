#pragma once

#include <iostream>
#include <memory>

#include "canvas.hpp"
#include "pixel.hpp"

namespace yadl
{
    class Shape
    {
    private:
        Shape() = default;
        Shape(const Shape &) = delete;
        Shape &operator=(const Shape &) = delete;
        Shape(Shape &&) = delete;
        Shape &operator=(Shape &&) = delete;
        ~Shape() = default;

        Pixel m_color{0, 0, 0, 255};
        size_t m_thicness{1};
        int32_t m_positionX{0};
        int32_t m_positionY{0};
        Canvas m_canvas{};

        using ActionFn = std::function<void(Pixel&)>;
        ActionFn m_blend = [&](Pixel &pixel) { pixel = pixel.Blend(m_color); };
        ActionFn m_add = [&](Pixel &pixel) { pixel += m_color; };
        ActionFn m_subtract = [&](Pixel &pixel) { pixel -= m_color; };
        ActionFn m_set = [&](Pixel &pixel) { pixel = m_color; };

        ActionFn m_action = m_set;

    public:
        Shape& DrawFilledRectangle(int32_t width, int32_t height);
        Shape& DrawFilledCircle(int32_t radius);


        inline static Shape &Get()
        {
            static Shape instance;
            return instance;
        }

        inline Shape &SetColor(Pixel color)
        {
            m_color = color;
            return *this;
        }

        inline Shape &SetThicness(size_t thicness)
        {
            m_thicness = thicness;
            return *this;
        }

        inline Shape &SetPosition(int32_t x, int32_t y)
        {
            m_positionX = x;
            m_positionY = y;
            return *this;
        }

        inline Shape &SetCanvas(Canvas &canvas)
        {
            m_canvas = canvas;
            return *this;
        }

        enum class Action
        {
            Blend,
            Add,
            Sub,
            Set
        };

        inline Shape &SetAction(Action action)
        {
            switch (action)
            {
            case Action::Blend:
                m_action = m_blend;
                break;
            case Action::Add:
                m_action = m_add;
                break;
            case Action::Sub:
                m_action = m_subtract;
                break;
            case Action::Set:
                m_action = m_set;
                break;
            default:
                break;
            }
            return *this;
        }


    };
}
