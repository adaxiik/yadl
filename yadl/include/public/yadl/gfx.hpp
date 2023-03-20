#pragma once
#include "context.hpp"

namespace yadl
{

    namespace gfx
    {
        /**
         * @brief Converts the image to grayscale.
         * @details Uses the (R+G+B)/3 formula.
         * 
         * @param ctx 
         */
        void Grayscale(Context &ctx);

        /**
         * @brief Inverts the image.
         * @details Uses the 1-RGB formula. 
         * 
         * @param ctx 
         */
        void Invert(Context &ctx);

        /**
         * @brief Applies a blur effect to the image.
         * 
         * @param ctx 
         * @param radius 
         */
        void Blur(Context &ctx, int32_t radius = 1);

        // TODOOO
        // static void Dither(Context& ctx);
    }
}
