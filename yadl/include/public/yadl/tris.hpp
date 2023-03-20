#pragma once
#include <cstdint>
#include "context.hpp"
namespace yadl
{
    namespace tris
    {
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
    }
} 
