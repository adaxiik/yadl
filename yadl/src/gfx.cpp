#include "gfx.hpp"

namespace yadl
{
    void gfx::Grayscale(Context &ctx)
    {
        auto &state = ctx.GetState();

        for (int32_t y = 0; y < state.canvas.GetHeight(); y++)
        {
            for (int32_t x = 0; x < state.canvas.GetWidth(); x++)
            {
                auto &pixel = state.canvas.RefPixel(x, y);
                uint8_t gray = (pixel.r + pixel.g + pixel.b) / 3;
                pixel.r = gray;
                pixel.g = gray;
                pixel.b = gray;
            }
        }
    }

    void gfx::Invert(Context &ctx)
    {
        auto &state = ctx.GetState();

        for (int32_t y = 0; y < state.canvas.GetHeight(); y++)
        {
            for (int32_t x = 0; x < state.canvas.GetWidth(); x++)
            {
                auto &pixel = state.canvas.RefPixel(x, y);
                pixel.r = 255 - pixel.r;
                pixel.g = 255 - pixel.g;
                pixel.b = 255 - pixel.b;
            }
        }
    }

    void gfx::Blur(Context &ctx, int32_t radius)
    {
        auto &state = ctx.GetState();

        Canvas temp(state.canvas.GetWidth(), state.canvas.GetHeight());

        for (int32_t y = 0; y < state.canvas.GetHeight(); y++)
        {
            for (int32_t x = 0; x < state.canvas.GetWidth(); x++)
            {
                int32_t r = 0, g = 0, b = 0, a = 0;
                int32_t count = 0;
                for (int32_t i = -radius; i <= radius; ++i)
                {
                    for (int32_t j = -radius; j <= radius; ++j)
                    {
                        int32_t x1 = x + i;
                        int32_t y1 = y + j;
                        if (x1 >= 0 && x1 < state.canvas.GetWidth() && y1 >= 0 && y1 < state.canvas.GetHeight())
                        {
                            auto pixel = state.canvas.GetPixel(x1, y1);
                            r += pixel.r;
                            g += pixel.g;
                            b += pixel.b;
                            a += pixel.a;
                            count++;
                        }
                    }
                }
                auto &pixel = temp.RefPixel(x, y);
                pixel.r = r / count;
                pixel.g = g / count;
                pixel.b = b / count;
                pixel.a = a / count;
            }
        }

        state.canvas.Paste(temp, 0, 0);
    }
}
