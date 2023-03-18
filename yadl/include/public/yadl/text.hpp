#pragma once
#include <string>

#include "context.hpp"
namespace yadl
{
    namespace text
    {
        /**
         * @brief Draw text
         * @warning Font must be set before calling this function, see Context.SetFont
         * 
         * @note Text position and scale is set by the context
         * 
         * 
         * @param ctx current context, see Context
         * @param text to draw
         */
        void DrawText(Context &ctx, const std::string &text);
    }
}