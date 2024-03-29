#include <iostream>
#include <memory>
#include <functional>

#include "canvas.hpp"
#include "pixel.hpp"
#include "context.hpp"

namespace yadl
{
    namespace shape
    {
        /**
         * @brief Draw a filled rectangle
         * 
         * @note Rectangle position is set by the context
         * 
         * @param ctx current context, see Context
         * @param width of the rectangle
         * @param height of the rectangle
         */
        void DrawFilledRectangle(Context& ctx, int32_t width, int32_t height);

        /**
         * @brief Draw a rectangle
         * 
         * @note Rectangle position and thickness is set by the context
         * 
         * @param ctx current context, see Context
         * @param width  of the rectangle
         * @param height of the rectangle
         */
        void DrawRectangle(Context& ctx, int32_t width, int32_t height);

        /**
         * @brief Draw a filled circle
         * 
         * @note Circle position is set by the context
         * @sa DrawFilledCircleAA
         * 
         * @param ctx current context, see Context
         * @param radius of the circle
         */
        void DrawFilledCircle(Context& ctx, int32_t radius);

        /**
         * @brief Draw a circle
         * 
         * @note Circle position and thickness is set by the context
         * @sa DrawCircleAA
         * 
         * @param ctx current context, see Context
         * @param radius of the circle
         */
        void DrawCircle(Context& ctx, int32_t radius);

        /**
         * @brief Draw a line
         * 
         * @note Line position and thickness is set by the context
         * @sa DrawLineAA
         * 
         * @param ctx current context, see Context
         * @param x end position of the line
         * @param y end position of the line
         */
        void DrawLine(Context& ctx, int32_t x, int32_t y);
        
        /**
         * @brief Draw a filled circle with anti-aliasing
         * 
         * @note Circle position is set by the context
         * @sa DrawFilledCircle
         * 
         * @param ctx current context, see Context
         * @param radius of the circle
         */
        void DrawFilledCircleAA(Context& ctx, int32_t radius);
        
        /**
         * @brief Draw a circle with anti-aliasing
         * 
         * @note Circle position and thickness is set by the context
         * @sa DrawCircle
         * 
         * @param ctx current context, see Context
         * @param radius of the circle
         */
        void DrawCircleAA(Context& ctx, int32_t radius);
        
        /**
         * @brief Draw a line with anti-aliasing
         * 
         * @note Line position and thickness is set by the context
         * @sa DrawLine
         * 
         * @param ctx current context, see Context
         * @param x end position of the line
         * @param y end position of the line
         */
        void DrawLineAA(Context& ctx, int32_t x, int32_t y);
    }

}