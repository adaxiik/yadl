#include "tris.hpp"

namespace yadl
{
    namespace tris
    {
       bool barycentric(int32_t x0, int32_t y0
                       , int32_t x1, int32_t y1
                       , int32_t x2, int32_t y2
                       , int32_t xp, int32_t yp
                       , float& u, float& v, float& w)
        {
            // literally just yoinked formula from here:
            // https://www.geogebra.org/m/c8DwbVTP
            float denominator = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
            u = ((y1 - y2) * (xp - x2) + (x2 - x1) * (yp - y2)) / denominator;
            v = ((y2 - y0) * (xp - x2) + (x0 - x2) * (yp - y2)) / denominator;
            w = 1.0f - u - v;

            return (u >= 0.0f && v >= 0.0f && w >= 0.0f);
        }

        void DrawTriangle(Context& ctx, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
        {
            auto& state = ctx.GetState();

            int32_t min_x = std::min(x0, std::min(x1, x2));
            int32_t max_x = std::max(x0, std::max(x1, x2));
            int32_t min_y = std::min(y0, std::min(y1, y2));
            int32_t max_y = std::max(y0, std::max(y1, y2));

            for (int32_t y = min_y; y <= max_y; y++)
            {
                for (int32_t x = min_x; x <= max_x; x++)
                {
                    float u, v, w;
                    if (barycentric(x0, y0, x1, y1, x2, y2, x, y, u, v, w))
                        state.action(state.canvas.RefPixel(x, y));
                }
            }
        }

        void DrawTriangle(Context& ctx, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel c0, Pixel c1, Pixel c2)
        {
            auto& state = ctx.GetState();
            Pixel original_color = state.color;
            int32_t min_x = std::min(x0, std::min(x1, x2));
            int32_t max_x = std::max(x0, std::max(x1, x2));
            int32_t min_y = std::min(y0, std::min(y1, y2));
            int32_t max_y = std::max(y0, std::max(y1, y2));

            for (int32_t y = min_y; y <= max_y; y++)
            {
                for (int32_t x = min_x; x <= max_x; x++)
                {
                    float u, v, w;
                    if (barycentric(x0, y0, x1, y1, x2, y2, x, y, u, v, w))
                    {
                        state.color = Pixel::Lerp(c0, c1, c2, u, v, w);
                        state.action(state.canvas.RefPixel(x, y));

                    }
                }
            }

            state.color = original_color;
        }
    }
}