#pragma once
#include "context.hpp"

namespace yadl
{

    namespace gfx
    {
        void Grayscale(Context &ctx);
        void Invert(Context &ctx);
        void Blur(Context &ctx, int32_t radius = 1);
        // TODOOO
        // static void Dither(Context& ctx);
    }
}
