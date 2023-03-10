#include "shape.hpp"
#include <functional>
namespace yadl
{
    Shape& Shape::DrawFilledRectangle(int32_t width, int32_t height)
    {
        for (int32_t y = m_positionY; y < m_positionY + height; ++y)
            for (int32_t x = m_positionX; x < m_positionX + width; ++x)
                m_action(m_canvas.RefPixel(x, y));
                
        return *this;
    }

    Shape& Shape::DrawFilledCircle(int32_t radius)
    {
        for (int32_t y = m_positionY - radius; y < m_positionY + radius; ++y)
            for (int32_t x = m_positionX - radius; x < m_positionX + radius; ++x)
                if ((x - m_positionX) * (x - m_positionX) + (y - m_positionY) * (y - m_positionY) <= radius * radius)
                    m_action(m_canvas.RefPixel(x, y));
        return *this;
    }


    Shape& Shape::DrawLine(int32_t x1, int32_t y1)
    {
        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
        // stupid and naive, but gets the job done
        
        int32_t x0 = m_positionX;
        int32_t y0 = m_positionY;
        int32_t dx = std::abs(x1 - x0);
        int32_t dy = -std::abs(y1 - y0);
        int32_t sx = x0 < x1 ? 1 : -1;
        int32_t sy = y0 < y1 ? 1 : -1;
        int32_t err = dx + dy;

        std::function<void(int32_t, int32_t)> draw;
        if (m_thickness == 1)
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                m_action(m_canvas.RefPixel(x0, y0));
            };
        }
        else
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                for (int32_t y = y0 - m_thickness; y < y0 + m_thickness; ++y)
                    for (int32_t x = x0 - m_thickness; x < x0 + m_thickness; ++x)
                        if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= m_thickness * m_thickness)
                            m_action(m_canvas.RefPixel(x, y));
            };
        }

        while (true)
        {
            draw(x0, y0);
            if (x0 == x1 && y0 == y1)
                break;
            int32_t e2 = 2 * err;
            if (e2 >= dy)
            {
                if (x0 == x1)
                    break;
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx)
            {
                if (y0 == y1)
                    break;
                err += dx;
                y0 += sy;
            }
        }

        return *this;

    }

    Shape& Shape::DrawRectangle(int32_t width, int32_t height)
    {
        if(m_thickness == 1)
        {
            for (int32_t y = m_positionY; y < m_positionY + height; y++)
            {
                m_action(m_canvas.RefPixel(m_positionX, y));
                m_action(m_canvas.RefPixel(m_positionX + width, y));
            }
            for (int32_t x = m_positionX; x < m_positionX + width; x++)
            {
                m_action(m_canvas.RefPixel(x, m_positionY));
                m_action(m_canvas.RefPixel(x, m_positionY + height));
            }
            
            return *this;
        }



        int32_t originalPositionX = m_positionX;
        int32_t originalPositionY = m_positionY;

        DrawLine(originalPositionX + width, originalPositionY);
        DrawLine(originalPositionX , originalPositionY + height);

        SetPosition(originalPositionX + width, originalPositionY + height);

        DrawLine(originalPositionX + width, originalPositionY);
        DrawLine(originalPositionX , originalPositionY + height);

        SetPosition(originalPositionX, originalPositionY);

        return *this;
    }

    Shape& Shape::DrawCircle(int32_t radius)
    {
       // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
       std::function<void(int32_t, int32_t)> draw;
        if (m_thickness == 1)
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                m_action(m_canvas.RefPixel(x0, y0));
            };
        }
        else
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                for (int32_t y = y0 - m_thickness; y < y0 + m_thickness; ++y)
                    for (int32_t x = x0 - m_thickness; x < x0 + m_thickness; ++x)
                        if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= m_thickness * m_thickness)
                            m_action(m_canvas.RefPixel(x, y));
            };
        }

        int32_t x = 0;
        int32_t y = radius;
        int32_t d = 3 - 2 * radius;

        auto drawCirclePart = [&](int32_t x, int32_t y)
        {
            draw(m_positionX + x, m_positionY + y);
            draw(m_positionX - x, m_positionY + y);
            draw(m_positionX + x, m_positionY - y);
            draw(m_positionX - x, m_positionY - y);
            draw(m_positionX + y, m_positionY + x);
            draw(m_positionX - y, m_positionY + x);
            draw(m_positionX + y, m_positionY - x);
            draw(m_positionX - y, m_positionY - x);
        };

        drawCirclePart(x, y);

        while (y >= x)
        {
            x++;

            if (d > 0)
            {
                y--;
                d = d + 4 * (x - y) + 10;
            }
            else
                d = d + 4 * x + 6;

            drawCirclePart(x, y);
        }

        return *this;
    }

}