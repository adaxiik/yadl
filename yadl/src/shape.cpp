#include "shape.hpp"

namespace yadl
{
    Shape& Shape::DrawFilledRectangle(int32_t width, int32_t height)
    {
        for (int32_t y = m_positionY; y < m_positionY + height; ++y)
            for (int32_t x = m_positionX; x < m_positionX + width; ++x)
                m_action(m_canvas.RefPixel(x, y));
                
        return *this;
    }
}