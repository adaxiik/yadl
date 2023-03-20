#include "tris.hpp"

namespace yadl
{
    namespace tris
    {

        Triangle::Triangle(const FloatTriangle& triangle)
            : x0(static_cast<int32_t>(triangle.x0))
            , y0(static_cast<int32_t>(triangle.y0))
            , x1(static_cast<int32_t>(triangle.x1))
            , y1(static_cast<int32_t>(triangle.y1))
            , x2(static_cast<int32_t>(triangle.x2))
            , y2(static_cast<int32_t>(triangle.y2))
        {}

        FloatTriangle::FloatTriangle(const Triangle& triangle)
            : x0(static_cast<float>(triangle.x0))
            , y0(static_cast<float>(triangle.y0))
            , x1(static_cast<float>(triangle.x1))
            , y1(static_cast<float>(triangle.y1))
            , x2(static_cast<float>(triangle.x2))
            , y2(static_cast<float>(triangle.y2))
        {}

        FloatTriangle Triangle::ToFloatTriangle() const
        {
            return FloatTriangle(static_cast<float>(x0), static_cast<float>(y0)
                               , static_cast<float>(x1), static_cast<float>(y1)
                               , static_cast<float>(x2), static_cast<float>(y2));
        }

        Triangle FloatTriangle::ToIntTriangle() const
        {
            return Triangle(static_cast<int32_t>(x0), static_cast<int32_t>(y0)
                          , static_cast<int32_t>(x1), static_cast<int32_t>(y1)
                          , static_cast<int32_t>(x2), static_cast<int32_t>(y2));
        }

        bool Barycentric(const Triangle& triangle
                       , int32_t xp, int32_t yp
                       , float& u, float& v, float& w)        
        {
            // literally just yoinked formula from here:
            // https://www.geogebra.org/m/c8DwbVTP
            int32_t x0 = triangle.x0;
            int32_t y0 = triangle.y0;
            int32_t x1 = triangle.x1;
            int32_t y1 = triangle.y1;
            int32_t x2 = triangle.x2;
            int32_t y2 = triangle.y2;

            float denominator = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
            u = ((y1 - y2) * (xp - x2) + (x2 - x1) * (yp - y2)) / denominator;
            v = ((y2 - y0) * (xp - x2) + (x0 - x2) * (yp - y2)) / denominator;
            w = 1.0f - u - v;

            return (u >= 0.0f && v >= 0.0f && w >= 0.0f);
        }

        bool Barycentric(int32_t x0, int32_t y0
                       , int32_t x1, int32_t y1
                       , int32_t x2, int32_t y2
                       , int32_t xp, int32_t yp
                       , float& u, float& v, float& w)
        {
            return Barycentric({x0, y0, x1, y1, x2, y2}, xp, yp, u, v, w);
        }


        void DrawTriangle(Context& ctx, const Triangle& triangle)
        {
            auto& state = ctx.GetState();

            int32_t min_x = std::min(triangle.x0, std::min(triangle.x1, triangle.x2));
            int32_t max_x = std::max(triangle.x0, std::max(triangle.x1, triangle.x2));
            int32_t min_y = std::min(triangle.y0, std::min(triangle.y1, triangle.y2));
            int32_t max_y = std::max(triangle.y0, std::max(triangle.y1, triangle.y2));

            for (int32_t y = min_y; y <= max_y; y++)
            {
                for (int32_t x = min_x; x <= max_x; x++)
                {
                    float u, v, w;
                    if (Barycentric(triangle, x, y, u, v, w))
                        state.action(state.canvas.RefPixel(x, y));
                }
            }
        }

        void DrawTriangle(Context& ctx, const Triangle& triangle, Pixel c0, Pixel c1, Pixel c2)
        {
            auto& state = ctx.GetState();
            Pixel original_color = state.color;

            int32_t min_x = std::min(triangle.x0, std::min(triangle.x1, triangle.x2));
            int32_t max_x = std::max(triangle.x0, std::max(triangle.x1, triangle.x2));
            int32_t min_y = std::min(triangle.y0, std::min(triangle.y1, triangle.y2));
            int32_t max_y = std::max(triangle.y0, std::max(triangle.y1, triangle.y2));

            for (int32_t y = min_y; y <= max_y; y++)
            {
                for (int32_t x = min_x; x <= max_x; x++)
                {
                    float u, v, w;
                    if (Barycentric(triangle, x, y, u, v, w))
                    {
                        state.color = Pixel::Lerp(c0, c1, c2, u, v, w);
                        state.action(state.canvas.RefPixel(x, y));

                    }
                }
            }

            state.color = original_color;
        }

        void DrawTriangle(Context& ctx, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
        {
            DrawTriangle(ctx, Triangle{x0, y0, x1, y1, x2, y2});
        }

        void DrawTriangle(Context& ctx, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel c0, Pixel c1, Pixel c2)
        {
            DrawTriangle(ctx, Triangle{x0, y0, x1, y1, x2, y2}, c0, c1, c2);
        }

    }
}