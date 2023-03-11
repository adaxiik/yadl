#include "text.hpp"
#include "font_internal.hpp"
#include <iostream>
namespace yadl
{
    Text &Text::DrawText(const std::string &text)
    {
        constexpr int32_t spaceWidth = 10;
        int32_t x = m_positionX;
        int32_t y = m_positionY;

        for (char c : text)
        {
            if(c == ' ')
            {
                x += static_cast<int32_t>(spaceWidth * m_scale);
                continue;
            }

            Canvas charCanvas = m_font.m_internal->GetCharCanvas(c);
            const auto& metrics = m_font.m_internal->GetGlyphMetrics(c);

            if(m_scale != 1.0f)
                charCanvas = charCanvas.Resize(m_scale);

            for (int32_t i = 0; i < charCanvas.GetWidth(); i++)
            {
                for (int32_t j = 0; j < charCanvas.GetHeight(); j++)
                {
                    Pixel charPixel = charCanvas.GetPixel(i, j);
                    if(charPixel.a > 0)
                    {
                        Pixel& canvasPixel = m_canvas.RefPixel(x + i - static_cast<int32_t>(metrics.m_bearingX * m_scale)
                                                             , y + j - static_cast<int32_t>(metrics.m_bearingY * m_scale));
                        Pixel targetColor = m_color;
                        targetColor *= charPixel;
                        canvasPixel = targetColor;
                    } 
                }
            }
            x += static_cast<int32_t>((metrics.m_width + metrics.m_bearingX) * m_scale);

        }



        return *this;
    }
}