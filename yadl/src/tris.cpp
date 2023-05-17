#include "tris.hpp"
#include <cstring>
#include "debug.hpp"

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

        void DrawTriangle(Context& ctx
                        , int32_t x0, int32_t y0
                        , int32_t x1, int32_t y1
                        , int32_t x2, int32_t y2)
        {
            DrawTriangle(ctx, Triangle{x0, y0, x1, y1, x2, y2});
        }

        void DrawTriangle(Context& ctx
                        , int32_t x0, int32_t y0
                        , int32_t x1, int32_t y1
                        , int32_t x2, int32_t y2
                        , Pixel c0, Pixel c1, Pixel c2)
        {
            DrawTriangle(ctx, Triangle{x0, y0, x1, y1, x2, y2}, c0, c1, c2);
        }


        void DrawTexturedTriangle(Context& ctx
                                , const Triangle& triangle
                                , const FloatTriangle& uv
                                , const Canvas& texture)
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
                        float tu = uv.x0 * u + uv.x1 * v + uv.x2 * w;
                        float tv = uv.y0 * u + uv.y1 * v + uv.y2 * w;

                        int32_t tx = static_cast<int32_t>(tu * texture.GetWidth());
                        int32_t ty = static_cast<int32_t>(tv * texture.GetHeight());

                        state.color = texture.GetPixel(tx, ty);
                        state.action(state.canvas.RefPixel(x, y));
                    }
                }
            }

            state.color = original_color;
        }

        void DrawTexturedTriangle(Context& ctx
                                , int32_t x0, int32_t y0
                                , int32_t x1, int32_t y1
                                , int32_t x2, int32_t y2
                                , float u0, float v0
                                , float u1, float v1
                                , float u2, float v2
                                , const Canvas& texture)
        {
            DrawTexturedTriangle(ctx
                               , Triangle{x0, y0, x1, y1, x2, y2}
                               , FloatTriangle{u0, v0, u1, v1, u2, v2}
                               , texture);
        }

        void DrawTriangleZ(Context& ctx, const FloatTriangle& triangle, float z0, float z1, float z2)
        {
            auto& state = ctx.GetState();

            int32_t min_x = std::min(triangle.x0, std::min(triangle.x1, triangle.x2));
            int32_t max_x = std::max(triangle.x0, std::max(triangle.x1, triangle.x2));
            int32_t min_y = std::min(triangle.y0, std::min(triangle.y1, triangle.y2));
            int32_t max_y = std::max(triangle.y0, std::max(triangle.y1, triangle.y2));

            float rz0 = 1.0f / z0;
            float rz1 = 1.0f / z1;
            float rz2 = 1.0f / z2;

            for (int32_t y = min_y; y <= max_y; y++)
            {
                for (int32_t x = min_x; x <= max_x; x++)
                {
                    float u, v, w;
                    if (Barycentric(triangle, x, y, u, v, w))
                    {
                        float current_z = rz0 * u + rz1 * v + rz2 * w;
                        float buffer_z = state.depthBuffer.GetDepth(x, y);

                        if (current_z > buffer_z)
                        {
                            state.depthBuffer.SetDepth(x, y, current_z);
                            state.action(state.canvas.RefPixel(x, y));
                        }
                    }
                }
            }
        }

        void DrawTriangleZ(Context& ctx
                         , const FloatTriangle& triangle
                         , float z0, float z1, float z2
                         , Pixel c0, Pixel c1, Pixel c2)
        {
            auto& state = ctx.GetState();

            int32_t min_x = std::min(triangle.x0, std::min(triangle.x1, triangle.x2));
            int32_t max_x = std::max(triangle.x0, std::max(triangle.x1, triangle.x2));
            int32_t min_y = std::min(triangle.y0, std::min(triangle.y1, triangle.y2));
            int32_t max_y = std::max(triangle.y0, std::max(triangle.y1, triangle.y2));

            float rz0 = 1.0f / z0;
            float rz1 = 1.0f / z1;
            float rz2 = 1.0f / z2;

            for (int32_t y = min_y; y <= max_y; y++)
            {
                for (int32_t x = min_x; x <= max_x; x++)
                {
                    float u, v, w;
                    if (Barycentric(triangle, x, y, u, v, w))
                    {
                        float current_z = rz0 * u + rz1 * v + rz2 * w;
                        float buffer_z = state.depthBuffer.GetDepth(x, y);

                        if (current_z > buffer_z)
                        {
                            state.depthBuffer.SetDepth(x, y, current_z);
                            state.color = Pixel::Lerp(c0, c1, c2, u, v, w);
                            state.action(state.canvas.RefPixel(x, y));
                        }
                    }
                }
            }
        }
        void DrawTexturedTriangleZ(Context& ctx
                                 , const FloatTriangle& triangle
                                 , float z0, float z1, float z2
                                 , const FloatTriangle& uv
                                 , const Canvas& texture)
        {
            auto& state = ctx.GetState();
            Pixel original_color = state.color;

            int32_t min_x = std::min(triangle.x0, std::min(triangle.x1, triangle.x2));
            int32_t max_x = std::max(triangle.x0, std::max(triangle.x1, triangle.x2));
            int32_t min_y = std::min(triangle.y0, std::min(triangle.y1, triangle.y2));
            int32_t max_y = std::max(triangle.y0, std::max(triangle.y1, triangle.y2));

            // https://gamedev.stackexchange.com/questions/121240/perspective-correct-texture-mapping
            // https://en.wikipedia.org/wiki/Texture_mapping
            
            FloatTriangle uv_div_z =   {uv.x0 / z0, uv.y0 / z0,
                                        uv.x1 / z1, uv.y1 / z1,
                                        uv.x2 / z2, uv.y2 / z2};

            // reciprocal of z
            float rz0 = 1.0f / z0;
            float rz1 = 1.0f / z1;
            float rz2 = 1.0f / z2;

            for (int32_t y = min_y; y <= max_y; y++)
            {
                for (int32_t x = min_x; x <= max_x; x++)
                {
                    float u, v, w;
                    if (Barycentric(triangle, x, y, u, v, w))
                    {
                        double current_z = rz0 * u + rz1 * v + rz2 * w;
                        float buffer_z = state.depthBuffer.GetDepth(x, y);
                    
                        if (current_z > buffer_z)
                        {
                            state.depthBuffer.SetDepth(x, y, current_z);
                            
                            double s = u * uv_div_z.x0 + v * uv_div_z.x1 + w * uv_div_z.x2;
                            double t = u * uv_div_z.y0 + v * uv_div_z.y1 + w * uv_div_z.y2;
                            
                            float tu = s / current_z;
                            float tv = t / current_z;

                            int32_t tx = static_cast<int32_t>(tu * texture.GetWidth());
                            int32_t ty = static_cast<int32_t>(tv * texture.GetHeight());

                            state.color = texture.GetPixel(tx, ty);
                            state.action(state.canvas.RefPixel(x, y));
                        }
                    }
                }
            }

            state.color = original_color;
        }
    }
}