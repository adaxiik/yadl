#include "text.hpp"
#include "font_internal.hpp"
#include <iostream>

namespace yadl
{
    void text::DrawText(Context& ctx, const std::string& text)
    {
        auto& state = ctx.GetState();
        
        if(state.font.GetInternal() == nullptr)
            throw std::runtime_error("Font not set");

        constexpr int32_t spaceWidth = 10;
        int32_t x = state.positionX;
        int32_t y = state.positionY + ctx.GetCurrentTextHeight();

        for (char c : text)
        {
            if(c == ' ')
            {
                x += static_cast<int32_t>(spaceWidth * state.fontScale);
                continue;
            }

            if(c == '\n')
            {
                x = state.positionX;
                y += ctx.GetCurrentTextHeight();
                continue;
            }

            // Canvas charCanvas = ctx.m_font.GetInternal()->GetCharCanvas(c);
            // const auto& metrics = ctx.m_font.GetInternal()->GetGlyphMetrics(c);
            Canvas charCanvas = state.font.GetInternal()->GetCharCanvas(c);
            const auto& metrics = state.font.GetInternal()->GetGlyphMetrics(c);

            if(state.fontScale != 1.0f)
                charCanvas = charCanvas.Resize(state.fontScale);

            for (int32_t i = 0; i < charCanvas.GetWidth(); i++)
            {
                for (int32_t j = 0; j < charCanvas.GetHeight(); j++)
                {
                    Pixel charPixel = charCanvas.GetPixel(i, j);
                    if(charPixel.a > 0)
                    {
                        Pixel& canvasPixel = state.canvas.RefPixel(x + i - static_cast<int32_t>(metrics.bearingX * state.fontScale)
                                                             , y + j - static_cast<int32_t>(metrics.bearingY * state.fontScale));
                        Pixel targetColor = state.color;
                        targetColor *= charPixel;
                        canvasPixel = targetColor;
                    } 
                }
            }
            x += static_cast<int32_t>((metrics.width + metrics.bearingX) * state.fontScale);
        }
    }


}