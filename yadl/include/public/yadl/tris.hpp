#pragma once
#include <cstdint>
#include <initializer_list>

#include "context.hpp"

#ifndef YADL_SIN
    #include <cmath>
    #define YADL_SIN(x) std::sin(x)
#endif

#ifndef YADL_COS
    #include <cmath>
    #define YADL_COS(x) std::cos(x)
#endif

namespace yadl
{
    namespace tris
    {

        struct FloatTriangle;
        struct Triangle;

        /**
         * @brief Structure that represents a triangle with integer coordinates.
         * 
         */
        struct Triangle
        {
            int32_t x0, y0;
            int32_t x1, y1;
            int32_t x2, y2;

            constexpr Triangle() : x0(0), y0(0), x1(0), y1(0), x2(0), y2(0) {}

            constexpr Triangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
                : x0(x0), y0(y0), x1(x1), y1(y1), x2(x2), y2(y2) {}

            Triangle(const FloatTriangle& triangle);

            FloatTriangle ToFloatTriangle() const;
        };

        /**
         * @brief Structure that represents a triangle with floating point coordinates.
         * 
         */
        struct FloatTriangle
        {
            float x0, y0;
            float x1, y1;
            float x2, y2;

            constexpr FloatTriangle() : x0(0.0f), y0(0.0f), x1(0.0f), y1(0.0f), x2(0.0f), y2(0.0f) {}

            constexpr FloatTriangle(float x0, float y0, float x1, float y1, float x2, float y2)
                : x0(x0), y0(y0), x1(x1), y1(y1), x2(x2), y2(y2) {}

            FloatTriangle(const Triangle& triangle);

            Triangle ToIntTriangle() const;
            
            /**
             * @brief Rotate the triangle around a point
             * 
             * @param[in] cx x coordinate of the center of rotation
             * @param[in] cy y coordinate of the center of rotation
             * @param[in] angle angle of rotation in radians
             */
            constexpr void Rotate(float cx, float cy, float angle)
            {
                const float s = YADL_SIN(angle);
                const float c = YADL_COS(angle);

                x0 -= cx;
                y0 -= cy;

                x1 -= cx;
                y1 -= cy;

                x2 -= cx;
                y2 -= cy;

                const float xnew0 = x0 * c - y0 * s;
                const float ynew0 = x0 * s + y0 * c;

                const float xnew1 = x1 * c - y1 * s;
                const float ynew1 = x1 * s + y1 * c;

                const float xnew2 = x2 * c - y2 * s;
                const float ynew2 = x2 * s + y2 * c;

                x0 = xnew0 + cx;
                y0 = ynew0 + cy;

                x1 = xnew1 + cx;
                y1 = ynew1 + cy;

                x2 = xnew2 + cx;
                y2 = ynew2 + cy;
            }
        };

        /**
         * @brief Barycentric coordinates of a point in a triangle
         * 
         * @sa https://en.wikipedia.org/wiki/Barycentric_coordinate_system
         * 
         * @param[in] x0 first vertex x coordinate
         * @param[in] y0 first vertex y coordinate
         * @param[in] x1 second vertex x coordinate
         * @param[in] y1 second vertex y coordinate
         * @param[in] x2 third vertex x coordinate
         * @param[in] y2 third vertex y coordinate
         * @param[in] xp x coordinate of the point that we want to know the barycentric coordinates
         * @param[in] yp y coordinate of the point that we want to know the barycentric coordinates
         * @param[out] u normalized barycentric coordinate of the first vertex
         * @param[out] v normalized barycentric coordinate of the second vertex
         * @param[out] w normalized barycentric coordinate of the third vertex
         * 
         * @return true if the point is inside the triangle
         * @return false if the point is outside the triangle
         */
        bool Barycentric(int32_t x0, int32_t y0
                       , int32_t x1, int32_t y1
                       , int32_t x2, int32_t y2
                       , int32_t xp, int32_t yp
                       , float& u, float& v, float& w);

        /**
         * @brief Barycentric coordinates of a point in a triangle
         * 
         * @sa https://en.wikipedia.org/wiki/Barycentric_coordinate_system
         * 
         * @param[in] triangle triangle
         * @param[in] xp x coordinate of the point that we want to know the barycentric coordinates
         * @param[in] yp y coordinate of the point that we want to know the barycentric coordinates
         * @param[out] u normalized barycentric coordinate of the first vertex
         * @param[out] v normalized barycentric coordinate of the second vertex
         * @param[out] w normalized barycentric coordinate of the third vertex
         * 
         * @return true if the point is inside the triangle
         * @return false if the point is outside the triangle
         */
        bool Barycentric(const Triangle& triangle
                       , int32_t xp, int32_t yp
                       , float& u, float& v, float& w);

        /**
         * @brief Draw a triangle
         * 
         * @note The triangle is filled with color that is set in the context
         * 
         * @param ctx context
         * @param x0 first vertex x coordinate
         * @param y0 first vertex y coordinate
         * @param x1 second vertex x coordinate
         * @param y1 second vertex y coordinate
         * @param x2 third vertex x coordinate
         * @param y2 third vertex y coordinate
         */
        void DrawTriangle(Context& ctx
                        , int32_t x0, int32_t y0
                        , int32_t x1, int32_t y1
                        , int32_t x2, int32_t y2);

        /**
         * @brief Draw a triangle
         * 
         * @note The triangle is filled with a linear gradient of the three given colors
         * 
         * @param ctx context
         * @param x0 first vertex x coordinate
         * @param y0 first vertex y coordinate
         * @param x1 second vertex x coordinate
         * @param y1 second vertex y coordinate
         * @param x2 third vertex x coordinate
         * @param y2 third vertex y coordinate
         * @param c0 color of the first vertex
         * @param c1 color of the second vertex
         * @param c2 color of the third vertex
         */
        void DrawTriangle(Context& ctx
                        , int32_t x0, int32_t y0
                        , int32_t x1, int32_t y1
                        , int32_t x2, int32_t y2
                        , Pixel c0, Pixel c1, Pixel c2);

        /**
         * @brief Draw a triangle
         * 
         * @note The triangle is filled with color that is set in the context
         * 
         * @param ctx context
         * @param triangle 
         */
        void DrawTriangle(Context& ctx, const Triangle& triangle);

        /**
         * @brief Draw a triangle
         * 
         * @note The triangle is filled with a linear gradient of the three given colors
         * 
         * @param ctx context
         * @param triangle 
         * @param c0 color of the first vertex
         * @param c1 color of the second vertex
         * @param c2 color of the third vertex
         */
        void DrawTriangle(Context& ctx, const Triangle& triangle, Pixel c0, Pixel c1, Pixel c2);

                  
        /**
         * @brief Draw a textured triangle
         * 
         * @param ctx context with the canvas to draw on
         * @param triangle screen coordinates of the triangle
         * @param uv texture coordinates of the triangle in range [0, 1], where (0, 0) is the top left corner of the texture
         * @param texture texture
         */
        void DrawTexturedTriangle(Context& ctx
                                , const Triangle& triangle
                                , const FloatTriangle& uv
                                , const Canvas& texture);


        /** 
         * @brief Draw a textured triangle
         * 
         * @note It just internally calls DrawTexturedTriangle(Context& ctx, const Triangle& triangle, const FloatTriangle& uv, const Canvas& texture)
         * 
         * @param ctx context with the canvas to draw on
         * @param x0 first vertex x coordinate
         * @param y0 first vertex y coordinate
         * @param x1 second vertex x coordinate
         * @param y1 second vertex y coordinate
         * @param x2 third vertex x coordinate
         * @param y2 third vertex y coordinate
         * @param u0 first vertex u coordinate
         * @param v0 first vertex v coordinate
         * @param u1 second vertex u coordinate
         * @param v1 second vertex v coordinate
         * @param u2 third vertex u coordinate
         * @param v2 third vertex v coordinate
         * @param texture texture
         */
        void DrawTexturedTriangle(Context& ctx
                                , int32_t x0, int32_t y0
                                , int32_t x1, int32_t y1
                                , int32_t x2, int32_t y2
                                , int32_t u0, int32_t v0
                                , int32_t u1, int32_t v1
                                , int32_t u2, int32_t v2
                                , const Canvas& texture);


        /**
         * @brief Draw a triangle with z-buffer
         * 
         * @warning The z-buffer must be set in the context
         * 
         * @param ctx context
         * @param triangle screen coordinates of the triangle
         * @param z0 z coordinate of the first vertex
         * @param z1 z coordinate of the second vertex
         * @param z2 z coordinate of the third vertex
         */
        void DrawTriangleZ(Context& ctx
                         , const FloatTriangle& triangle
                         , float z0, float z1, float z2);


        /**
         * @brief Draw a triangle with z-buffer
         * 
         * @warning The z-buffer must be set in the context
         * 
         * @param ctx context
         * @param triangle screen coordinates of the triangle
         * @param z0 z coordinate of the first vertex
         * @param z1 z coordinate of the second vertex
         * @param z2 z coordinate of the third vertex
         * @param c0 color of the first vertex
         * @param c1 color of the second vertex
         * @param c2 color of the third vertex
         */
        void DrawTriangleZ(Context& ctx
                         , const FloatTriangle& triangle
                         , float z0, float z1, float z2
                         , Pixel c0, Pixel c1, Pixel c2);

        /**
         * @brief Draw a textured triangle with z-buffer
         * 
         * @note It does perspective correct interpolation of the texture coordinates
         * 
         * @param ctx context with the canvas to draw on
         * @param triangle screen coordinates of the triangle
         * @param z0 z coordinate of the first vertex
         * @param z1 z coordinate of the second vertex
         * @param z2 z coordinate of the third vertex
         * @param uv texture coordinates of the triangle in range [0, 1], where (0, 0) is the top left corner of the texture
         * @param texture texture
         */
        void DrawTexturedTriangleZ(Context& ctx
                                 , const FloatTriangle& triangle
                                 , float z0, float z1, float z2
                                 , const FloatTriangle& uv
                                 , const Canvas& texture);
    }
} 
