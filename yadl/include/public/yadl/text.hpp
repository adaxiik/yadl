#pragma once
#include <string>

#include "context.hpp"
namespace yadl
{
    class Text
    {
    private:
        Text() = delete;
        Text(const Text &) = delete;
        Text &operator=(const Text &) = delete;
        Text(Text &&) = delete;
        Text &operator=(Text &&) = delete;
        ~Text() = delete;
    public:
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
        static void DrawText(Context& ctx, const std::string& text);
    };
}