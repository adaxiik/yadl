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
                    if(charPixel.a > 128)
                    {
                        Pixel& canvasPixel = state.canvas.RefPixel(x + i - static_cast<int32_t>(metrics.bearingX * state.fontScale)
                                                             , y + j - static_cast<int32_t>(metrics.bearingY * state.fontScale));
                        Pixel targetColor = state.color;
                        targetColor *= charPixel;
                        canvasPixel = targetColor;
                    } 


                    // TODO: Fix this
                    // const float distance = (float)charCanvas.GetPixel(i, j).a / 255.0f;
                    // const float buffer = 0.2f;
                    // const float gamma = 1.0f;

                    // smoothstep:
                    // t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
                    // return t * t * (3.0 - 2.0 * t);

                    // alpha = smoothstep(buffer - gamma, buffer + gamma, distance)
                    
                    // float t = std::clamp((distance - (buffer - gamma)) / (buffer + gamma - (buffer - gamma)), 0.0f, 1.0f);
                    // float alpha = t * t * (3.0f - 2.0f * t);

                    // if(alpha > 0.0f && alpha < 1.0f)
                    // {
                    //     Pixel& canvasPixel = state.canvas.RefPixel(x + i - static_cast<int32_t>(metrics.bearingX * state.fontScale)
                    //                                          , y + j - static_cast<int32_t>(metrics.bearingY * state.fontScale));
                    //     Pixel targetColor = state.color;
                    //     targetColor.a = static_cast<uint8_t>(targetColor.a * alpha);
                    //     canvasPixel.Blend(targetColor);
                    // }

                

                }
            }
            x += static_cast<int32_t>((metrics.width + metrics.bearingX + metrics.bearingX) * state.fontScale);
        }
    }


}